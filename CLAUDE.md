# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

TextQuest is a terminal game/game engine (C++23) that renders a Wolfenstein-style ray-cast 3D view entirely with ASCII characters, drawn through **notcurses**. All computation is on the CPU (a GPU migration is planned).

## Build & run

```sh
make            # optimized build (-O2) -> ./tq, objects in build/
make debug      # ASan + UBSan build (-O1) -> ./tq-debug, objects in build-debug/
make clean
./tq            # run from the repo root
```

- Dependencies: `notcurses-core` (via pkg-config) and `ncursesw`.
- The Makefile has **no header dependency tracking** — after editing a header, run `make clean` (or touch the dependent .cpp files) or you will link stale objects.
- New `.cpp` files anywhere under `src/` are picked up automatically (`find src -name '*.cpp'`). `-Isrc/Headers` is on the include path, but `src/GameSpecific/Headers/` is not, so those files use relative includes.
- There is no test suite and no linter config.
- Run from the repo root: `Logger` writes to `./logs/{common,core,input,render,texprep,scene}.log` (truncated each run), and `src/TextQuest.cpp` loads textures from **hardcoded absolute paths** under `/home/attila/Kitchen/TextQuest/Resources/Textures/`.
- Controls (see `NotcursesInputHandler.cpp`): `w/s` forward/back, `a/d` strafe, `q/e` turn, `i` shoot, `x` quit. Key release events are used, so the terminal needs the kitty keyboard protocol; `~GameEngine` pops that mode on exit.
- Only the Linux/notcurses path is live. The `Windows*` and `NCurses*` renderer/input `.cpp` files are still matched by the build glob, but they are neutralized by `#if` platform guards or commented out. They are stale and not expected to compile if re-enabled.

## Texture pipeline

Textures are plain-text ASCII art generated offline from images:

- `Resources/TexGen.py` (run via `Resources/run_texgen.fish` from inside `Resources/`, uses `Resources/venv` with Pillow) converts every image in `Resources/TextureSource/` into `Resources/Textures/<Name>/<Name>lv0.txt … lv7.txt`: 8 progressively darker variants for distance shading, with power-of-2 dimensions.
- At runtime, `SceneManager::loadResources` reads a directory into a `stem -> contents` map. `TextQuest.cpp` sorts the keys so that `lv0` (brightest) comes first, then passes the vector to `uploadTextureVecForWall` (wall map char) or an `Entity` constructor.
- Texture format: rows joined with `'\n'`, all the same width; width = index of the first `'\n'`. A space `' '` is transparent for entity sprites.
- `TextureMapper` owns the shading variants ("mipmaps") and does nearest-neighbour sampling. `RenderAssetManager` maps distance to a shading index through thresholds set with `GameEngine::setDistanceShadingThresholds`.

## Architecture

`src/TextQuest.cpp` (`main`) acts as the "game" for now: it builds the map string, loads textures, creates entities, and calls `GameEngine::run_game()`. `GameEngine::sceneMan()` / `inputHandler()` are temporary hooks, pending a proper game-programmer API.

**Frame loop** (`src/Core/Core.cpp`, `GameEngine::run_game`):
1. `IInputHandler::ReceiveInput()` updates key state.
2. `SceneManager::process(dt)` runs `EntityManager::process`, which goes through `resolveStates` (dead entities are moved to (-1,-1)), then `resolveMovement`, `resolveProjectiles`, and `resolveVisibility`.
3. `RayCastingProcess`: a DDA ray per screen column writes wall/floor chars into the engine's `char* screen` buffer and fills `_zBuffer` with one depth value per column.
4. `EntityProjectionProcess`: billboards entity sprites far-to-near using the camera-plane inverse matrix and z-buffer tests per column.
5. `IRenderer::OverwriteBuffer(screen)`, then `PrintDebugInfo`, then `PrintBuffer()` (`notcurses_render`).

**Scene layer** (`src/Scene/`):
- `SceneManager` owns `EntityManager`, `MapManager`, and an `optional<Camera>`. The camera is created lazily by `setCameraFollow`, and it holds a reference to an entity's `Transform`. The camera must be set before `run_game`.
- `SceneManager` implements `IEntitySceneChannel` (`canMoveTo`), which is the only way `EntityManager` can query the map.
- `RenderAssetManager` is a `friend` of `SceneManager`'s ctor and keeps const refs to its `MapManager`/`EntityManager`. It is the rendering side's read-only view of the scene.
- Map: a flat `std::string` indexed `y * width + x`. `'.'` is floor; any other char is a wall whose texture is looked up by that char in `MapManager::_wallTexMappers`.

**Entities**:
- `Entity` is a single concrete class. Per-entity logic is composed in via `std::unique_ptr<IBehaviorController>` (`Tick` returns the desired new position; there are also `onCollision` / `onVisible` / `onHit` hooks). Don't subclass `Entity`. Game-specific controllers live in `src/GameSpecific/`.
- **Entity ID == index in `EntityManager::_entityContainer`.** `addEntity` moves the passed-in entity into the vector and assigns its ID; the caller's object is left moved-from. `removeEntity` doesn't erase anything, it only moves the entity to (-1,-1). Code throughout indexes `_entityContainer[id]` directly, so don't reorder or erase from the vector without reworking IDs (there's a TODO about this).
- Movement is intent-based. All controllers tick first, then intents are validated per axis against walls (`TerrainCollidable`) and other entities' collision radii, firing `onCollision` on both sides.
- Projectiles are hitscan. Only the closest entity along the ray (distance squared, max 20 units) gets `onHit`.
- Angle convention: direction = `(sin(angle), cos(angle))`.

**UI system** (`src/UIDisplay/`, in progress and not yet wired into `GameEngine`):
- Responsibilities are split: `UIManager` owns and manages overlays/elements through integer `UIElementHandle`s. `IRenderer` only creates an overlay plane (`createOverlay(Rect)` returns an `OverlayId`) and replaces its content (`setOverlayContent`, which erases and rewrites the whole plane).
- `UIElement` follows the same composition pattern as `Entity` (behavior via `IUIElementBehavior`, no subclassing). It is move-only and keeps `design` (static frame) separate from `content`, which is spliced into `writableArea` on every render.
- `NotcursesRenderer::createOverlay` rejects rects that overlap an existing overlay or have non-positive coordinates or size. `destroyOverlay` doesn't exist yet.

**Logging**: `Logger::GetInstance()->log(msg, LogType::X, LogLevel::Y)` (a static singleton). Use it rather than stdout/stderr while the game is running, because notcurses owns the terminal.
