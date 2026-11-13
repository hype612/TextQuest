#include "SceneBuilder.h"
#include "../MapGen/MapGenerator.h"
#include "Entity.h"
#include "IInputHandler.h"
#include "Logger.h"
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

SceneBuilder::SceneBuilder(IInputHandler &input, std::string textureRoot)
    : _input(input), _textureRoot(std::move(textureRoot)) {}

void SceneBuilder::registerEnemyArchetype(std::string name,
                                          EnemyArchetype type) {
  auto [it, succ] =
      _enemyArchetypes.try_emplace(std::move(name), std::move(type));

  // it is never intended to overwrite the value of an existing
  // archetype for now. So it logs and throws.
  if (!succ) {
    Logger::GetInstance()->log(
        "registerEnemyArchetype: insertion failed due to key duplication. The "
        "duplicated key: " +
            it->first + ".",
        LogType::CORE, LogLevel::ERROR);
    Logger::GetInstance()->forceFlush();
    throw std::invalid_argument(
        "registerEnemyArchetype: duplicate archetype key: " + it->first);
  }
}

void SceneBuilder::spawnEnemy(const std::string &typeKey, SceneManager &scene,
                              vec2f pos, const Difficulty &difficulty,
                              const ControllerFactory &makeCtrl) {
  auto it = _enemyArchetypes.find(typeKey);
  if (it == _enemyArchetypes.end()) {
    Logger::GetInstance()->log(
        "spawnEnemy: function was called with non-existant key: " + typeKey,
        LogType::CORE, LogLevel::ERROR);
    Logger::GetInstance()->forceFlush();
    throw std::out_of_range("spawnEnemy: unknown archetype key: " + typeKey);
  }
  // The registered archetype holds the base stats. Scale a copy with the
  // difficulty chosen for this run.
  EnemyArchetype a = it->second;
  a.health *= difficulty.enemyHealth;
  a.damage *= difficulty.enemyDamage;
  a.fovDegrees *= difficulty.fovDegrees;
  a.viewDistance *= difficulty.viewDistance;
  a.moveSpeed *= difficulty.enemySpeed;
  a.shootCooldown *= difficulty.attackCooldown;
  a.reactionDelay *= difficulty.reactionDelay;
  a.missChance = std::clamp(a.missChance * difficulty.missChance, 0.f, 1.f);

  Entity e(makeCtrl(a), Transform{pos, 1.f}, a.textures, a.health,
           a.collisionRadius, a.hitRadius, false, a.fovDegrees, a.viewDistance);
  e.setMoveSpeedAllDirectons(a.moveSpeed);
  e.setTurnSpeedAlldirections(a.turnSpeed);
  scene.AddEntity(e);
}

// Draws one rand() per leaf room for its wall theme. Depends on the stream
// seeded in build(): the number of rand() calls before this must be the same
// on every run for a given seed, or wall themes change between replays.
std::string
SceneBuilder::serializeThemedMap(const std::vector<std::vector<Tile>> &map,
                                 const std::vector<Rect> &leafRooms,
                                 const std::vector<char> &wallThemes) const {
  if (wallThemes.empty()) {
    Logger::GetInstance()->log("serializeThemedMap: no wall themes given",
                               LogType::CORE, LogLevel::ERROR);
    Logger::GetInstance()->forceFlush();
    throw std::invalid_argument("serializeThemedMap: no wall themes given");
  }
  int mapHeight = static_cast<int>(map.size());
  int mapWidth = mapHeight > 0 ? static_cast<int>(map[0].size()) : 0;

  std::string out(static_cast<size_t>(mapWidth) * mapHeight, '.');
  for (int y = 0; y < mapHeight; y++)
    for (int x = 0; x < mapWidth; x++)
      if (map[y][x] == Tile::WALL)
        out[y * mapWidth + x] = wallThemes[0];

  for (const Rect &room : leafRooms) {
    char theme = wallThemes[rand() % wallThemes.size()];
    for (int y = 0; y < static_cast<int>(room.height); y++) {
      for (int x = 0; x < static_cast<int>(room.width); x++) {
        int gx = room.x + x, gy = room.y + y;
        if (map[gy][gx] == Tile::WALL)
          out[gy * mapWidth + gx] = theme;
      }
    }
  }
  return out;
}

std::vector<std::string>
SceneBuilder::loadTextureVec(const std::string &name) const {
  const std::string dir = (std::filesystem::path(_textureRoot) / name).string();
  const std::unordered_map<std::string, std::string> texs = readTextureDir(dir);
  std::vector<std::string> keys;
  keys.reserve(texs.size());
  for (const auto &[key, value] : texs)
    keys.push_back(key);
  std::sort(keys.begin(), keys.end());
  std::vector<std::string> out;
  out.reserve(texs.size());
  for (const auto &key : keys)
    out.push_back(texs.at(key));
  Logger::GetInstance()->log(
      dir + " texture count: " + std::to_string(out.size()), LogType::TEXPREP,
      LogLevel::INFO);
  return out;
}

std::unordered_map<std::string, std::string>
SceneBuilder::readTextureDir(const std::string &dir) const {
  std::unordered_map<std::string, std::string> textures;
  for (const auto &entry : std::filesystem::directory_iterator(dir)) {
    if (!std::filesystem::is_regular_file(entry))
      continue;
    std::ifstream file(entry.path());
    if (file.fail()) {
      Logger::GetInstance()->log(
          "failed to open texture file: " + entry.path().string() +
              ", reading next texture file..",
          LogType::TEXPREP, LogLevel::ERROR);
      continue;
    }
    std::stringstream contents;
    contents << file.rdbuf();
    std::string key = entry.path().stem().string();
    if (textures.contains(key)) {
      Logger::GetInstance()->log("texture already present, skipping: " +
                                     entry.path().string(),
                                 LogType::TEXPREP, LogLevel::ERROR);
      continue;
    }
    textures[key] = contents.str();
  }
  return textures;
}

SceneBuilder::BuiltScene SceneBuilder::build(unsigned int seed,
                                             const Difficulty &difficulty) {
  Logger *l = Logger::GetInstance();
  auto scene = std::make_shared<SceneManager>();
  SceneManager &sceneMan = *scene;

  // The whole build is deterministic per seed because everything draws from
  // the global rand() stream, seeded once here. Any rand() call during the
  // build must happen the same number of times on every run (no dependence on
  // timing, input, threads or running enemies), or the same seed will give a
  // different scene (e.g. different wall textures on a replay).
  srand(seed);
  MapGenerator mg(Rect{0, 0, 50, 50}, part_params{.3f, .7f, 10, 10, 20, 20}, 5,
                  .35f);

  std::vector<std::string> walltexV = loadTextureVec("wall");
  sceneMan.uploadTextureVecForWall('#', walltexV);
  std::vector<std::string> tnttexV = loadTextureVec("tnt");
  sceneMan.uploadTextureVecForWall('T', tnttexV);

  std::string map = serializeThemedMap(mg.map(), mg.leafRooms(), {'#', 'T'});
  sceneMan.initializeNewMap(map, 50, 50);
  l->log("map inited", LogType::CORE, LogLevel::INFO);

  mg.generateSpawnPoints(difficulty.enemyDensity,
                         /*boss_room_count=*/1);
  vec2f playerSpawn{-1.f, -1.f};
  for (const SpawnPoint &sp : mg.spawnPoints())
    if (sp.type == SpawnType::PLAYER)
      playerSpawn = sp.pos;

  std::string intex = " \n";
  Transform init{playerSpawn, 0.f};
  l->log("before player construction", LogType::CORE, LogLevel::INFO);
  auto playerCtrl = std::make_unique<PlayerBehaviorController>(_input);
  PlayerBehaviorController *playerCtrlPtr = playerCtrl.get();
  Entity p(std::move(playerCtrl), init, &intex, _playerMaxHp, 0.15f, 0.3f, true,
           0.f, 0.f);
  p.setMoveSpeedAllDirectons(3.5f);
  p.setTurnSpeedAlldirections(2.5f);
  sceneMan.AddEntity(p);
  l->log("player added to entities", LogType::CORE, LogLevel::INFO);

  // Enemies are spawned after the player so that the player keeps entity ID 0
  // (the camera follows it).
  EnemyBehaviorController *demonCtrlPtr = nullptr;
  for (const SpawnPoint &sp : mg.spawnPoints()) {
    if (sp.type == SpawnType::PLAYER)
      continue;
    vec2f spawnF = sp.pos;
    EnemyBehaviorController *rawCtrl = nullptr;
    auto makeCtrl = [&](const EnemyArchetype &a) {
      auto ctrl = std::make_unique<EnemyBehaviorController>(
          mg.generatePatrolPoints(spawnF, 3), a.shootCooldown, 3.f,
          a.reactionDelay, a.missChance);
      rawCtrl = ctrl.get();
      return ctrl;
    };
    if (sp.type == SpawnType::BOSS) {
      spawnEnemy("demon", sceneMan, spawnF, difficulty, makeCtrl);
      demonCtrlPtr = rawCtrl;
    } else {
      spawnEnemy("ghoul", sceneMan, spawnF, difficulty, makeCtrl);
    }
  }

  // ==================
  // Win cond setup
  // ==================
  sceneMan.setWinCondition([](const SceneManager &sc_man) -> bool {
    return std::ranges::all_of(sc_man.entities(), [](const Entity &e) {
      return e.isPlayer() || e.health() <= 0;
    });
  });
  sceneMan.setCameraFollow(sceneMan.entityAtId(0).transform());
  sceneMan.setCameraFovDegrees(90);
  l->log("camera follow set", LogType::CORE, LogLevel::INFO);
  return {scene, playerCtrlPtr, demonCtrlPtr};
}
