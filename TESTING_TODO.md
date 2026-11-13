# TextQuest test suite TODO

Plan for an automated test suite for the engine and the game, with separate suites for each.
This is part of an ELTE BSc thesis ("Játékfejlesztés Ray-Casting Technológia Használatával"),
whose topic declaration promises code quality and documentation, so the tests and their
results will also be described in the thesis.

## What the thesis requires (ELTE IK, Programtervező informatikus BSc)

Sources: the faculty's thesis page (inf.elte.hu, "Programtervező informatikus BSc szakdolgozat
tudnivalók") and the guide it links, "Általános tanácsok szakdolgozat és diplomamunka
készítéséhez", section 5.2.2.

- The *Fejlesztői dokumentáció* must contain "a tesztelési tervet és a tesztelés eredményeit".
  One of the five grading criteria is "a tesztelés minőségi értékelése". A criterion that scores
  zero means the thesis is not recommended for defense.
- The guide asks for "Tesztelés módja, futási eredmények riportjával és azok kiértékelésével":
  - "Minden osztályodra legyen tesztelés is. Egység tesztek, integrációs tesztek, rendszer és
    elfogadási tesztek."
  - Where automation isn't possible, write a *teszt jegyzőkönyv* that states the function under
    test and its expected output.
  - When testing the UI manually, consider UX: is everything in the right place, is it logical
    to use, and is it accessible.
- The choice of testing methodology counts as a design decision, so the thesis should justify
  it against alternatives.

This plan is organized to match those requirements. Each phase produces the material for one
subsection of the thesis's "Tesztelés" section:

| Thesis subsection | Produced by |
|---|---|
| Tesztelési terv (method, tools, levels, justification) | this file + step 5.1 |
| Egységtesztek | Phase 1, Phase 2 |
| Integrációs tesztek | Phase 3 |
| Rendszer- és elfogadási tesztek | Phase 4 |
| Eredmények és kiértékelés | Phase 5 |

**Test levels used in this plan:**
- **Unit**: one class under test. Its collaborators are fakes from `tests/support/` or plain value
  types. The real `SceneManager` may stand in as the map backend, because it is simpler than a
  fake.
- **Integration**: several real project classes working together (builder + map generator +
  entities, engine loop + scene + UI, ray caster + scene + assets).
- **System**: the whole game headless, driven by scripted input.
- **Acceptance**: manual test log against the features in the topic declaration and the user
  documentation, including UX checks.

## How to work through this file (instructions for the agent)

1. Read `CLAUDE.md` first. It describes the architecture, the build and the conventions.
2. Work through the steps **in order, one step at a time**. Each step has an ID (e.g. `1.4`).
3. Before you start a step, tell the user in 2-4 lines what you are about to change.
4. Do the step. Keep it to that step's scope. If you notice something outside the scope, write
   it down and mention it, but don't do it.
5. When the step is done:
   - show the diff of what changed (`git diff` for tracked files, and the full contents or
     `git diff --no-index /dev/null <file>` for new files)
   - run `make test` (and `make` if production code changed) and show the relevant output,
     whether it passed or failed
   - for refactor steps, ask the user to run `./tq` and check that the game behaves the same as
     before
6. **Stop and wait for the user's explicit approval before you go on to the next step.** If
   the user asks for changes, make them, show the new diff, and ask again.
7. Once a step is approved, tick its checkbox in this file (`- [ ]` → `- [x]`).
8. Don't commit unless the user asks you to.
9. **Never fix production code silently.** When a test exposes a bug in `src/`, stop and report
   it, with the test, the observed behavior and the expected behavior. Ask whether to fix it
   now, or to keep the test and mark it `DISABLED_` with a comment that refers to the
   "Known issues" section. Bugs that are already known are listed in that section. Every bug
   found goes into that table, because step 5.2 reports them as test results.
10. Test names should describe the behavior they check, e.g.
    `EntityManager.DiagonalMoveIntoWallSlidesAlongIt`. Put an "Arrange / Act / Assert" structure
    in each test body. In the thesis, the test names serve as the test-case list.
11. **Keep tests simple.** Test the behavior listed in the step and nothing more. Don't build
    support code that only one test uses. Prefer a few clear asserts over a framework.

## Fixed decisions (don't revisit these)

- **Build system: Make.** No CMake. Extend the existing `Makefile`.
- **Test framework: GoogleTest 1.18**, already installed and found through
  `pkg-config gtest gmock gtest_main`. Hand-written fakes are preferred over GMock mocks.
- **RNG: keep the global `rand()`.** Tests make it deterministic with `srand(seed)`. It is
  acceptable that results differ between libc implementations or architectures. Don't inject
  `std::mt19937`.
- **`Rect::overlaps` counts touching rects as overlapping.** This is intended. Write a test that
  pins it down, but don't change the code.
- **Use only public APIs in tests.** No `#define private public`, and don't add `friend` for
  tests. If something can only be tested through a private member, either test it through the
  public API or extract it in a dedicated refactor step.
- **Logger:** it opens `./logs/*.log` during static initialization, so tests must run from the
  repo root and `logs/` must exist. `make test` takes care of both.
- Tests must not need a terminal. Nothing in the test binaries may call `notcurses_init`,
  `notcurses_core_init`, or construct `NotcursesRenderer` / `NotcursesInputHandler`. These
  classes are covered by the manual tests (4.2).
- The engine/game separation is enforced at link time: the engine test binary links the engine
  library only.

## Target layout

```
tests/
  support/        # fakes and helpers shared by both suites
  engine/         # engine tests: must not include anything from src/GameSpecific/
  game/           # game tests
  fixtures/
    Textures/     # tiny 4x4 textures with the same layout as Resources/Textures
  MANUAL_TESTS.md # acceptance test log (step 4.2)
  TEST_REPORT.md  # results and evaluation (step 5.2)
```

---

## Phase 0: infrastructure

### - [x] 0.1 Split the Makefile into libraries and add test targets

- `ENGINE_SRCS` = every `src/**/*.cpp` except `src/GameSpecific/**` and `src/TextQuest.cpp`.
- `GAME_SRCS` = `src/GameSpecific/**/*.cpp`.
- Build `build/libtqengine.a` and `build/libtqgame.a`. `./tq` = `TextQuest.o` + game lib +
  engine lib, in that link order. Do the same for the debug build (`build-debug/`, `tq-debug`).
- Add two test binaries:
  - `build/tests/tq-engine-tests`: `tests/engine/**/*.cpp` + `tests/support/**/*.cpp`, linked
    against the **engine lib only**. If this fails to link, the engine has picked up a
    dependency on game code, and that is a bug.
  - `build/tests/tq-game-tests`: `tests/game/**/*.cpp` + `tests/support/**/*.cpp`, linked
    against the game lib + engine lib.
  - Both get `-Isrc/Headers -Itests/support`, the gtest cflags from pkg-config, `gtest_main`,
    and the notcurses libs (static-lib members like `Core.o` can pull them in).
- Targets:
  - `make test`: builds both binaries, runs `mkdir -p logs`, runs both from the repo root, and
    exits non-zero if either fails
  - `make test-debug`: the same with the ASan/UBSan flags, objects in `build-debug/tests/`
  - `make clean`: also removes the test outputs
- Keep `-MMD -MP` dependency tracking for the test objects.
- Add one trivial sanity test to each binary so the wiring can be verified.
- Acceptance: `make clean && make && make test && make test-debug` all succeed, and `./tq` still
  runs as before. The user checks the last part.

### - [x] 0.2 Test support code (`tests/support/`)

Only what several test files need:
- `FakeRenderer : IRenderer`: records every call (`createOverlay`, `setOverlay`,
  `setOverlayRegion`, `destroyOverlay`, `setOverlayVisible`) with its arguments in public
  vectors. `createOverlay` hands out increasing IDs, or returns `-1` when a `rejectNext` flag is
  set. It has a configurable screen size. `Init`, `OverwriteBuffer` (copies the buffer) and
  `PrintBuffer` do nothing beyond recording.
- `ScriptedInput : IInputHandler`: `setKeyDown(EntityAction, bool)` and `pressRaw(key)`. Tests
  change the key state between ticks themselves.
- `ScriptedController : IBehaviorController`: returns a queued intent from `Tick`, and records
  `onCollision` (with `collidableKind()`), `onVisible` and `onHit` calls into a struct that the
  test owns and that outlives the entity.
- Recording observers: `RecordingHealthObserver`, `RecordingShootObserver` and
  `RecordingEnemyStateObserver`.
- Helpers:
  - `makeEntity(pos, angle, opts...)`, which uses a 1×1 texture `"x\n"`. Texture format: rows
    joined with `'\n'`, and width = index of the first `'\n'`.
  - `makeMap(std::vector<std::string> rows)` → `{std::string map, int w, int h}`, to set up a
    real `SceneManager`.
- `tests/fixtures/Textures/{wall,tnt,ghoul,demon}/<name>lv0.txt … lv7.txt`: 4×4 textures. Each
  shading level gets a distinct character (`'0'`…`'7'`) so tests can tell which level was
  used. Same layout as `Resources/Textures/`.
- Acceptance: the support code compiles, and the sanity tests use at least `FakeRenderer` and
  `makeEntity`.

---

## Phase 1: unit tests (egységtesztek) that need no refactor

### - [x] 1.1 Value types (`tests/engine/ValueTypesTest.cpp`)

- `vec2f`: `+ - *`, `dot`, `cross`, `+= -= *=`, `==` within epsilon (1e-5) and `!=`;
  `normalize()` on a non-zero vector gives length 1, and on a zero vector leaves it unchanged.
- `vec2f::normalized()`: write a test asserting that it **does not** change `*this`. It
  currently does (see Known issues), so stop and ask as described in instruction 9.
- `Rect::overlaps`: disjoint, overlapping, nested, and **touching edges → true (intended)**.
  `contains(Rect)`: inner, equal and partly outside. `contains(px, py)`: the right and bottom
  edges are exclusive.
- `EntityDistance`: `operator<` and the order produced by `std::sort`.

### - [ ] 1.2 `TextureMapper` (`tests/engine/TextureMapperTest.cpp`)

- Width and height parsed from a texture.
- Mask: `' '` → 0, other characters → 1.
- `nxyInterpolationScale`: upscaling 2×2 → 4×4 duplicates each texel, and each row ends in
  `'\n'`.
- `getTexColumnAt`: the length equals the visible range, hitpoint 0 samples column 0 and
  hitpoint 0.999 samples the last column, and the shading index picks the right variant (use
  the `'0'`…`'7'` fixture textures).

### - [ ] 1.3 `MapManager` and `SceneManager` map queries (`tests/engine/MapTest.cpp`)

- `isOutOfBounds` at the borders and just inside them. `isWall` for `'.'`, `'#'`, another wall
  character, and out of bounds (currently returns false; pin that down).
- Texture lookup per map character.
- `SceneManager::canMoveTo`: the radius corners next to a wall, radius 0, out of bounds.
- `setWinCondition`: `sceneOver()` and `playerWon()` become true after `process` once the
  condition holds. `playerDiedNotify` sets `sceneOver` but not `playerWon`.

### - [ ] 1.4 `EntityManager` (`tests/engine/EntityManagerTest.cpp`)

Use a real `SceneManager` built with `makeMap`, and `ScriptedController` entities.
- IDs: `addEntity` assigns 0, 1, 2, …. Adding an entity whose ID ≠ -1 throws
  `std::invalid_argument`. `removeEntity` moves the entity to (-1,-1) and keeps the count.
- Movement: a free move is applied; a diagonal move into a wall slides along the wall and
  calls `onCollision(WALL)`; two entities closer than the sum of their radii are blocked, and
  both get `onCollision(ENTITY)`.
- States: a dead enemy is moved to (-1,-1). A dead player calls `playerDiedNotify`.
- Projectiles: only the closest entity along the ray gets `onHit`; no hit through a wall; no
  hit beyond about 20 units; the shooter never hits itself.
- Visibility: `onVisible` inside FOV and view distance with a clear line; nothing outside the
  FOV, beyond the view distance, or behind a wall.
- `entitiesSortedByDistanceTo` is ascending.

### - [ ] 1.5 `RenderAssetManager` and `Camera` (`tests/engine/RenderAssetManagerTest.cpp`, `CameraTest.cpp`)

- Shading off → always variant 0. Shading on with thresholds `{2,4,6}` → the right variant just
  below and just above each threshold. Observe it through `scaledEntityTex` / `charColumn` with
  the `'0'`…`'7'` fixtures, since `getShadingIndex` is private.
- `Camera`: degree ↔ radian conversion; `setFollow` changes which `Transform` is followed.

### - [ ] 1.6 `UIElement` and `UIManager` (`tests/engine/UITest.cpp`)

- `UIElement`:
  - a new element is dirty; `takeDesign()` / `takeContent()` return a value once, then nullopt
  - `updateDesign` / `updateContent` make it dirty again
  - `setColorForSec` / `setContentForSec` override until `tick` has used up the time, then fall
    back to the base values
- `UIManager` with `FakeRenderer`:
  - `addElement` returns the overlay ID, or -1 when the renderer rejects it
  - the first `process` writes the design and the content; a later `process` with no changes
    writes nothing; a content-only change writes only `setOverlayRegion`
  - `removeElement` → `destroyOverlay`, then `elementAt` == nullptr; `clear` destroys every
    overlay

### - [ ] 1.7 `PlayerBehaviorController` (`tests/game/PlayerControllerTest.cpp`)

- With `ScriptedInput`:
  - forward intent = position + (sin a, cos a)·speed·dt; backward is the opposite
  - strafe is perpendicular to forward; turning changes the angle and not the position
  - no keys → returned position == current position
- Shooting: the shoot observer fires on the first press, not again within 0.3 s, and again
  after 0.3 s.
- `onHit`: health drops by the other entity's `dmg()` and the health observer receives the new
  value.

### - [ ] 1.8 `EnemyBehaviorController` (`tests/game/EnemyControllerTest.cpp`)

Call `srand(<fixed>)` in `SetUp()`, because the jitter and miss rolls use `rand()`.
- It starts IDLE and patrols toward patrol point 0, then goes through the points in ping-pong
  order. With fewer than 2 points it stays put.
- Transitions: IDLE→CHASE when the player is seen beyond shooting distance; IDLE→ATTACK when seen
  within it; CHASE↔ATTACK as the distance changes. The state observer is called once for each
  change.
- `missChance = 1` → never `shooting()`. `missChance = 0` → shoots after the cooldown.
- `onHit` lowers health.

### - [ ] 1.9 Presenters and observers (`tests/game/PresenterTest.cpp`)

Use a real `UIManager` with `FakeRenderer`.
- `HealthBarPresenter`: at 0, 50 and 100 of 100, and clamped above max and below 0: the line
  is exactly the writable-area width and has the right number of `#`.
- `MinimapPresenter`: the arrow for the 4 main angles; walls `#`; living enemies `e`; dead
  enemies not shown; the window is clipped at the map edges.
- `DamageFlashPresenter`: flashes on damage; document whether it also flashes on healing.
- `ViewmodelPresenter` and `EnemyStatePresenter`: they react to their events.
- `HealthObserverFanOut`: forwards to every added observer, in order.

### - [ ] 1.10 Difficulty invariants (`tests/game/DifficultyTest.cpp`)

- EASY ≤ NORMAL ≤ HARD for `enemyDamage`, `enemyHealth`, `enemySpeed`, `fovDegrees`,
  `viewDistance` and `enemyDensity`. EASY ≥ NORMAL ≥ HARD for `attackCooldown`, `reactionDelay`
  and `missChance`.

### - [ ] 1.11 `BspNode` (`tests/game/BspTest.cpp`)

For seeds 0..99, each with `srand(seed)`, partition a 50×50 rect with the `part_params` used in
`SceneBuilder::build`:
- every leaf lies inside the parent, and the leaf areas add up to the parent area
- leaves don't overlap (use strict interior overlap, since `Rect::overlaps` counts touching)
- leaves respect the min sizes

Use `SCOPED_TRACE(seed)` so a failure reports the seed.

### - [ ] 1.12 `MapGenerator` property tests (`tests/game/MapGeneratorTest.cpp`)

For seeds 0..199, with the same parameters as `SceneBuilder::build`, and `SCOPED_TRACE(seed)`:
- the border is all `WALL`
- every `FLOOR` cell is reachable from the player spawn by 4-neighbour flood fill
- after `generateSpawnPoints(density, 1)`: exactly one PLAYER spawn, ≤ 1 BOSS, and every spawn
  is on a FLOOR cell
- `generatePatrolPoints(spawn, 3)`: every point is on FLOOR
- the same seed twice gives an identical map and identical spawns

---

## Phase 2: refactors for testability, and their unit tests

Each refactor must **preserve behavior**. After each one: show the diff, run `make test`, and
ask the user to play `./tq` briefly. Write the tests for a refactor in the step after it, not
in the same step. The refactors are worth a short paragraph in the thesis ("testability"
design decisions).

### - [ ] 2.1 Refactor: `CellularAutomata` with a given starting grid

- Add a constructor that takes the initial `std::vector<std::vector<Tile>>`. The existing noise
  constructor is unchanged, or delegates to it.

### - [ ] 2.2 `CellularAutomata` tests (`tests/game/CellularAutomataTest.cpp`)

- The neighbour count in the middle, at an edge and at a corner (document how out-of-range
  neighbours are counted).
- On a hand-made 5×5 grid, one generation turns cells with ≥ `min` wall neighbours into walls
  and leaves the others unchanged ("sticky walls").

### - [ ] 2.3 Refactor: extract `SeedStore`

- Move `TextQuestCore::loadLastSeed` / `saveSeed` into
  `src/GameSpecific/Game/SeedStore.{h,cpp}`, which takes the file path in its constructor.
  `TextQuestCore` holds one with `"last_seed.txt"`. This is the `SeedStore` item in the user's
  `TextQuestCore` refactor notes.

### - [ ] 2.4 `SeedStore` tests (`tests/game/SeedStoreTest.cpp`)

Use a temp file under `std::filesystem::temp_directory_path()`:
- save→load round trip; missing file → nullopt; garbage or empty file → nullopt.

### - [ ] 2.5 Refactor: inject renderer and input into `GameEngine`

- Add `GameEngine(std::unique_ptr<IRenderer>, std::unique_ptr<IInputHandler>)`. The existing
  default constructor builds the notcurses pair and delegates to it.
- Replace the raw `new`/`delete` of `_renderer`, `_inputHandler` and `_uimanager` with
  `unique_ptr`s. Keep the destruction order: `UIManager` before the renderer. The terminal
  reset in `~GameEngine` (`\033[<u`) should only run for the notcurses path (e.g. move it into
  `~NotcursesInputHandler`, or make it a flag). Ask the user which.

### - [ ] 2.6 Refactor: pull out a single frame step

- Move the body of the `run_game` loop into `void GameEngine::tick(float dt)`. `run_game` keeps
  the clock and the `while (_engineRunning)` loop and calls `tick`.
- Add `bool GameEngine::running() const`, and a `begin()` (scene check, buffer allocation) that
  `run_game` calls before the loop. Keep `run_game`'s public behavior the same.

### - [ ] 2.7 Refactor: pull out the `RayCaster`

- Move `RayCastingProcess`, `RenderCol` and `EntityProjectionProcess` into a `RayCaster` class
  (`src/Headers/RayCaster.h`, `src/Rendering/RayCaster.cpp`). Its input is a const
  `SceneManager&`, a `RenderAssetManager&`, a `const Camera&` and a screen size. It writes into
  a buffer the caller owns, or returns a `Frame { cells, width, height, zBuffer }`. Pick
  whichever needs the fewer changes, and say why.
- `GameEngine::tick` uses it. The output must be byte-identical: before the refactor, dump a
  frame from a fixed seed and pose to a scratch file, dump it again after, and compare. Don't
  commit the dump.
- Remove the dead `sideStr` switch inside the DDA loop only if the user agrees.

---

## Phase 3: integration tests (integrációs tesztek)

### - [ ] 3.1 `SceneBuilder` (`tests/game/SceneBuilderTest.cpp`)

Builder + map generator + entities + texture loading together. Construct it with
`ScriptedInput` and `textureRoot = "tests/fixtures/Textures/"`, and register `"ghoul"` and
`"demon"` archetypes made from `loadTextureVec`, as `TextQuestCore` does.
- `registerEnemyArchetype` with a duplicate key throws `std::invalid_argument`.
- `loadTextureVec` returns 8 variants, lv0 first.
- `build(seed, NORMAL)`: entity 0 is the player with 100 HP; the camera follows entity 0;
  `enemyCtrl` is non-null exactly when a boss spawned; the map is 50×50.
- The same seed with HARD gives enemies 1.5× NORMAL HP.
- `build(seed)` twice gives the same map string and the same entity positions.
- Setting the health of every enemy to 0 → `process` → `playerWon()`.

### - [ ] 3.2 Engine loop (`tests/engine/GameEngineTest.cpp`)

`GameEngine` + `SceneManager` + `UIManager` with `FakeRenderer` + `ScriptedInput`.
- `begin` without a scene, or without a map, throws `std::logic_error`.
- `setScene` takes effect at the next `tick`.
- `onFrame` receives the dt that was passed in.
- When the scene ends, `onSceneOver` fires exactly once, then `postSceneOver` fires on every
  later tick.
- A raw `'x'` stops the engine (`running()` becomes false).
- `runMenu` returns when the callback returns false.

### - [ ] 3.3 Ray caster (`tests/engine/RayCasterTest.cpp`)

`RayCaster` + `SceneManager` + `RenderAssetManager` + `Camera`, on small hand-written maps with
the fixture textures:
- facing a wall at distance d, the middle column's wall height ≈ `screenHeight / d` (±1), and
  zBuffer[middle] ≈ d
- a scene that is mirror-symmetric around the view direction gives a mirror-symmetric frame
- an open direction (no wall within 50) gives the no-hit column
- a sprite in front of a wall is drawn; one behind a wall, or behind the camera, is not
- spaces in a sprite are transparent

---

## Phase 4: system and acceptance tests (rendszer- és elfogadási tesztek)

### - [ ] 4.1 Headless scripted playthrough (`tests/game/PlaythroughTest.cpp`), system test

- `SceneBuilder` with the fixtures, `build(42, NORMAL)`, and a `GameEngine` with `FakeRenderer`
  + `ScriptedInput`.
- Script: hold forward for 2 s, turn right for 1 s, shoot 5 times, strafe. Step 600 ticks at
  dt = 1/60.
- After every tick: the player's cell is never a wall, every living enemy stays inside the map,
  and no entity's health exceeds its starting value.
- Set every enemy's health to 0 → the next tick ends the scene with `playerWon()`.
- Set the player's health to 0 → the scene ends without a win, and `onSceneOver` fires once.

### - [ ] 4.2 Acceptance test log (`tests/MANUAL_TESTS.md`), *teszt jegyzőkönyv*

For what automation can't cover (the notcurses renderer and input, menus, the real terminal).
A table with the columns ID | Feature | Precondition | Steps | Expected result | Actual result
| Pass/Fail. Derive the rows from the features in the topic declaration and the user
documentation, so each feature has at least one acceptance test:
- start menu and difficulty selection
- movement and turning with the kitty keyboard protocol, including key release
- shooting, enemy damage, the health bar and the damage flash
- the minimap and the boss state display
- winning, dying and the end screen; retry replays the same seed
- quitting with `x` restores the terminal
- terminal resize, and a terminal that is too small

Add a short UX section, as the guide asks: is the HUD readable, are the controls discoverable
from the start screen, is the damage flash distinguishable without relying on color alone.

Leave the "Actual result" and "Pass/Fail" columns empty for the user to fill in.

---

## Phase 5: results and evaluation (eredmények és kiértékelés)

### - [ ] 5.1 Coverage (`make coverage`)

- Needs `gcovr`, which is not installed. Ask the user to install it (`sudo pacman -S gcovr`, or
  `pipx install gcovr`). Don't install it yourself.
- Build into `build-coverage/` with `--coverage -O0`, run both test binaries, and generate an
  HTML report at `build-coverage/coverage/index.html` plus a text summary.
- Exclude `tests/`, `src/Rendering/Notcurses*`, `src/Input/Notcurses*`, the stale
  `Windows*`/`NCurses*` files and `src/TextQuest.cpp`.
- Show the summary to the user. The target is about 80% line coverage on engine logic. Don't
  write filler tests just to reach the number; list the uncovered parts instead.

### - [ ] 5.2 Test report (`tests/TEST_REPORT.md`)

The raw material for the thesis's "Eredmények és kiértékelés" subsection. Keep it short:
- **Testing plan summary**: the four levels, the tools (GoogleTest, Make, fakes, `srand`
  pinning, ASan/UBSan, gcovr) and a few lines on why each was chosen over its alternatives
  (e.g. Catch2/doctest, CMake/CTest, GMock mocks, an injected RNG).
- **Run results**: the number of tests per level and per suite, pass/fail, run time, from the
  latest `make test` and `make test-debug` (copy the gtest summary lines).
- **Class coverage matrix**: every class in `src/` (except the stale `Windows*`/`NCurses*`
  ones) → the test file(s) or manual test IDs that cover it, or the reason it has none (e.g.
  `Logger`, see K5). This answers "Minden osztályodra legyen tesztelés is".
- **Coverage**: the summary from 5.1.
- **Bugs found by testing**: the Known issues table with its final status.
- **Evaluation**: what the tests give confidence in, what they don't (e.g. visual output is
  checked only analytically and manually), and what would be done next.

### - [ ] 5.3 (Optional) Continuous integration. Ask the user before creating this.

- `.github/workflows/tests.yml` on push and PR: install a C++23-capable g++ (Ubuntu 24.04's
  default g++ may need to be replaced with g++-14), notcurses dev packages, libgtest-dev and
  libgmock-dev, then run `make test` and `make test-debug`. Check that the package names exist
  before you rely on them.
- Add a status badge to `README.md`.

### - [ ] 5.4 (Optional) Ray-cast benchmark

Only if the user asks for it. A small `std::chrono` harness that measures the frame time of
`RayCaster` on a fixed scene at 3 screen sizes, as the CPU baseline for the planned GPU
migration.

### - [ ] 5.5 (Optional, low priority) Random-input robustness test (`tests/game/MonkeyTest.cpp`)

Probably won't be done. It is kept here so it isn't forgotten. Skip it unless the user asks
for it explicitly. The guide's test levels are covered without it, because 4.1 is already the
system test.
- For seeds 1..3, with `srand(seed)`: random key states each frame for 3,000 ticks at fixed dt.
- The same invariants as 4.1. Nothing may throw.
- It must pass under `make test-debug` (ASan/UBSan clean).

---

## Known issues found during planning

Pin each one down with a test when you reach it, then ask the user whether to fix it.

| # | Location | Issue | Status |
|---|---|---|---|
| K1 | `src/Headers/Vec2f.h` `normalized()` | Changes `*this` (`return {x /= len, y /= len}`) and isn't `const` | **fixed** (step 1.1) |
| K2 | `src/Headers/Rect.h` `overlaps` | Touching rects count as overlapping | **intended, keep** |
| K3 | `src/GameSpecific/MapGen/cellularAutomata.cpp` | `rand() % 100 <= threshold` gives (threshold+1)% walls | open |
| K4 | `src/Scene/EntityManager.cpp` `projectileHitDistSq` | The direction check only rejects vectors longer than unit length; shorter ones pass | open |
| K5 | `src/Core/Logger.cpp` `~Logger` | `delete instancePtr` inside the destructor → double delete if it ever runs | open |
| K6 | `src/GameSpecific/Game/SceneBuilder.cpp` `loadTextureVec` | Lexicographic key sort: `lv10` < `lv2` | open (not hit today: 8 levels) |
| K7 | `src/GameSpecific/EntityControllers/PlayerBehaviorController.cpp` | Includes `<ncurses.h>` without using it | open (ask the user during step 1.7) |

---

delete this file after everything is done
