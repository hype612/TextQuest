---
title: TextQuest TODO
tags: [textquest, thesis, todo]
source: Thesis topic registration (2024-12-21), "Játékfejlesztés Ray-Casting Technológia Használatával"
---

# TextQuest TODO

Goal: a complete, playable game on the custom ray-casting engine, covering what the
thesis proposal promises. The engine part is essentially done, so what remains is the
game around it. Read `CLAUDE.md` first for architecture and build notes.

Time estimates are rough hours of focused work, assuming AI assistance for
boilerplate. They exclude writing the thesis text itself.

## Proposal coverage

| Promise | State |
|---|---|
| Ray-cast pseudo-3D with textures | Done |
| User interaction | Done (move, turn, shoot) |
| Dynamic game space | Missing (map is a hardcoded string in `src/TextQuest.cpp`) |
| Enemy behavior patterns | Partial (enemy chases, never attacks) |
| Difficulty settings | Missing |
| Feedback mechanisms | Partial (health bar only) |
| Complete playable game | Missing (no win/lose/restart) |
| Code quality and docs | Partial |

## Critical path (do in this order)

### 1. Enemy attacks and states (~4-6 h)
- [x] Give `IstvanBehaviorController` states: idle, chase, attack
  (`src/GameSpecific/EntityControllers/IstvanBehaviorController.cpp`).
- [x] In attack, call `self.setShooting(true)` on a cooldown, like the player's `_shootcd`.
  This reaches `PlayerBehaviorController::onHit`, which drives the health bar.
- [x] Wall line-of-sight for enemies: `EntityManager::resolveVisibility` already does a
  DDA obstruction check before calling `onVisible`, so an enemy that only attacks from
  `onVisible` never shoots through a wall.
- [x] (Separate gameplay bug, optional) `resolveProjectiles` has no wall check, so the
  player's shots can hit enemies behind walls.
- [x] Remove the per-frame `Logger` call in `IstvanBehaviorController::Tick` (log spam),
  initialise `_target`, `_targetAngle` and `_a_idx`, and stop `onCollision` from
  damaging the enemy itself when it touches an entity.
- Done when: an enemy in view walks up, stops, and drains the player's health bar.
  It does not hit through walls.

### 2. Win, lose and restart (~4-6 h)
- [x] Lose: player health <= 0. Today `EntityManager::resolveStates` just moves the
  entity to (-1,-1); detect this in the game layer instead.
- [x] Win: all non-player entities are dead.
- [x] Show a "YOU DIED" / "YOU WIN" message with a new presenter (see
  `src/GameSpecific/UIControllers/HealthBarPresenter.cpp` as the pattern).
- [ ] game end check and win cond check is done but is not in a separate presenter. If needed, move it there. win condition is not tested or propagated.
- [x] Restart or quit on a key press.
- IT IS Done|Done when: the game can be finished and replayed without restarting the process by hand.

### 3. Difficulty settings (~2-3 h)
- [ ] `Difficulty` struct with multipliers: enemy damage, enemy health, enemy speed,
  attack cooldown.
- [ ] Select via command-line argument (`./tq hard`), or a small menu.
- [ ] Apply when constructing entities in the game setup.
- Done when: easy, normal and hard visibly change how hard the same map is.

### 4. Dynamic game space (~3-5 h)
- [ ] Generate a map at startup: recursive-backtracker maze from a seed, returned as the
  same flat `y * width + x` string that `SceneManager::initializeNewMap` takes.
- [ ] Spawn the player and enemies at random floor cells (not inside walls, not too close
  to the player).
- [ ] Fallback if generation looks bad: load maps from files plus a next-level step.
- Done when: each run (or each seed) gives a different playable layout.

### 5. Feedback mechanisms (~2-3 h)
- [ ] Damage feedback (message or short flash) when the player is hit.
- [ ] Kill / hit messages, reusing the message presenter from step 2.
- Done when: the player gets a visible reaction to every hit taken.

### 6. Code quality and documentation (~9-14 h)
- [x] Replace hardcoded `/home/attila/Kitchen/TextQuest/...` texture paths in
  `src/TextQuest.cpp` with repo-relative paths (~1 h).
- [ ] Move game setup out of `main` into a `Game` class under `src/GameSpecific/` (~2-3 h).
- [x] Add header dependency tracking to the `Makefile` (`-MMD -MP`) (~0.5 h).
- [x] `~GameEngine` does not `delete _uimanager` (~0.25 h).
- [ ] Delete, or clearly mark, the stale `Windows*` and `NCurses*` files (~1 h).
- [ ] Turn `CLAUDE.md` and the design decisions (presenter/observer UI, intent-based
  movement, composition over subclassing) into the thesis architecture section (~4-8 h).
- [ ] Optional: a few tests, e.g. for `HealthBarPresenter::renderBar` and map generation.

### 7. Stretch: sprite facing and state animation (~6-8 h, not in the proposal)
Only start this after steps 1-6 are done, and only if still on schedule at the Oct 18
decision point. It must be finished before the Oct 25 feature freeze, otherwise drop it.
- [ ] Facing indicator: pick a sprite variant (front / back to start) from the angle
  between the entity's facing and the direction to the camera, so the player can tell
  whether an enemy faces them (~2-3 h).
- [ ] State-based animation component: per-state frame lists (idle, chase, attack, dead),
  a frame timer, driven by the state `IstvanBehaviorController` already exposes (~4-5 h).
- [ ] Texture storage per entity becomes `state -> facing -> frame -> shading`; extend
  `TexGen.py` accordingly. Build the small version first, generalise later.
- Art: AI-generated or sourced from elsewhere. Record the source and licence of every
  sourced asset, and note AI-generated ones for the AI usage declaration.
- Done when: an enemy visibly changes frames per state and its sprite differs when it
  faces the player versus away.

## Known issues (small, fix opportunistically)
- [ ] In `GameEngine::run_game`, `_uimanager->process()` runs after `PrintBuffer()`,
  so overlay changes show one frame late. Move it before `PrintBuffer()`.
- [x] `sceneMan.AddEntity(p)` leaves the caller's `Entity` moved-from. Anything done to
  the local after that (e.g. `p.setHealth(50)`) is lost. Use `entityAtId(...)`.
- [x] `Entity::setHealth` does not notify anyone. Only `PlayerBehaviorController::onHit`
  updates the health observer, so pickups or other damage sources would not reach the bar.
- [x] `Rect::overlaps` treats touching edges as overlapping, so `createOverlay` rejects
  elements that merely touch.
- [x] `GameEngine::screnSize` is misspelled (`src/Headers/Core.h` and `Core.cpp`).

## Schedule (8 h/week, from 2026-09-21; HARD deadline: Tuesday 2026-12-01, no extension)

Source: https://www.inf.elte.hu/szakdolgozat-diplomamunka-leadasanak-rendje
Upload to Neptun as one PDF/ZIP named `NAME_NEPTUN_SZAKDOLGOZAT.pdf`. Include the
thesis with cover page, the topic registration form and the AI tool usage declaration.
Before uploading, Neptun asks for the final title, 3-5 keywords and a max-500-character abstract.

| Week | Dates | Code | Writing |
|---|---|---|---|
| 1 | Sep 21-27 | Step 1 (6 h), repo-relative paths, Makefile deps (1.5 h) | - |
| 2 | Sep 28-Oct 4 | Step 2 (6 h), leak and UI-order fixes (0.5 h) | Outline, start ray-casting chapter (1.5 h) |
| 3 | Oct 5-11 | Step 3 (3 h), `Game` class (3 h) | 2 h |
| 4 | Oct 12-18 | Step 4 (5 h) | 3 h |
| 5 | Oct 19-25 | Step 5 (3 h), stale-file cleanup (1 h), step 7 stretch only if ahead | 4 h |
| | **Oct 25** | **Feature freeze**, bug fixes only | |
| 6-8 | Oct 26-Nov 15 | Playtesting and bug fixes only | ~8 h/week, **full draft by Nov 15** |
| 9 | Nov 16-22 | Gameplay recording, clean-clone build check | Supervisors review the draft, you revise |
| 10 | Nov 23-29 | - | Final revisions, AI declaration, cover page, PDF, Neptun metadata. **Upload by Nov 27**, not Dec 1 evening |
| | **Dec 1** | Deadline | |

- Do the path fix first: it lets you build and run on another machine (e.g. at work).
  The terminal must support the kitty keyboard protocol (see `CLAUDE.md`).
- Send an outline to the supervisors in October and a real draft by Nov 15 at the latest.
- If a step slips, cut step 7 first, then step 5, then step 4, never the writing time.
  Decide on Oct 18.
- Keep notes on where AI assistance was used, so the declaration is accurate.

## Estimates

| Block | Hours |
|---|---|
| 1-5, game features | ~15-23 |
| 6, quality and docs | ~9-14 |
| Total | ~25-35, plus playtesting and debugging slack |

## Stage 2

- [x] Add `setWinCondition(std::function<bool(const SceneManager&)>)` to `SceneManager`.
- [x] Poll it once per frame in `SceneManager::process(dt)`, alongside `resolveStates`; on
  true, set a `_playerWon` bool and `sceneOver()` (no new notifier method).
- [x] Add `playerWon()` query on `SceneManager`, queried by the game layer once the run
  loop ends (mirrors polling `sceneOver()`, no addition to `IEntitySceneChannel`).
- [x] Wire a win predicate (all non-player entities dead) in `src/TextQuest.cpp`.

## Stage 3

Swappable scenes: the game owns scenes, the engine only shows the active one. The player
can be carried from one scene to the next. Nothing here is implemented yet.

- [x] `SceneManager`: delete copy and move operations and construct it on the heap
  (`EntityManager` and `RenderAssetManager` hold references into it, so its address must
  stay stable).
- [x] `GameEngine`: replace the `SceneManager` member with a `std::shared_ptr<SceneManager>`
  set through `setActiveScene(...)`. The game is the primary owner. The engine's copy keeps
  the old scene alive while its own callback triggers the swap.
- [x] Apply the swap at the top of the next loop iteration, not mid-frame. On swap: rebind
  `RenderAssetManager` to the new scene (member bound at construction -> `optional` or
  `rebind`), reapply the stored shading settings, clear `_sceneOverFired`, check the
  camera is set.
- [TBD] Move the scene-over callback from the engine to the scene:
  `SceneManager::setOnSceneOver(std::function<void(bool won)>)`. The engine keeps only the
  fire-once boolean and calls it from the existing scene-over branch (after `process()`).
- [x] `SceneManager::extractPlayer()`: move the player `Entity` out and reset its ID to -1.
  The game re-adds it to the new scene as the first entity (ID 0), sets the spawn transform
  itself (`addEntity` touches only the ID) and re-points the camera.
- [x] Restart: done differently from the original design. The engine calls
  `setPostSceneOver` every frame while the scene is over, and the game polls
  `rawKeyPressed('r')` there and calls `startScene`. It has no effect mid-game. No
  `restartPressed()` accessor or `setOnRestart` exists.
- [ ] Reset input state (`_quitPressed`, held keys) on scene swap.
- [x] `IRenderer::destroyOverlay` (+ notcurses implementation) and
  `UIManager::removeElement`, plus `UIManager::clear()`. On restart `startScene` clears
  every overlay and rebuilds the whole HUD (health bar and Istvan state display) together
  with the scene, so the presenters are re-attached to the new controllers.
- [ ] Game side (`src/TextQuest.cpp`): split `main` into per-level builders returning a
  `shared_ptr<SceneManager>`. Each sets its own win condition, on-over and restart
  callbacks. Restart builds a fresh player, advancing carries the existing player (HP
  included, decided by the game, never by the engine). Partly done: `buildScene`,
  `buildHud` and `startScene` exist, but the callbacks are still set in `main`, and there
  is only one level.
- Done when: dying and pressing `r` rebuilds the level in the same process without
  reinitialising notcurses, and winning can swap to a second map with the same player.

## Cut for now (not in the proposal)
- Viewport plane / Wolfenstein-style HUD panel with nested overlays
- Generic `UIPresenter` base class (extract only after a third presenter)
- Terminal resize handling
- GPU migration and the Windows path
