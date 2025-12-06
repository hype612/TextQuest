#include "../Headers/SceneManager.h"

SceneManager::SceneManager(EntityManager &entityMan, MapManager &mapMan)
    : _entityManager(entityMan), _mapManager(mapMan),
      _player(-1, -1, 0.f, 60, mapMan) {
  EngineState::globalSceneManager = this;
}

void SceneManager::process() {
  _entityManager.process();
  //_eventManager.process();
}

// Map Related functions
void SceneManager::initializeNewMap(std::string &map, int mapWidth,
                                    int mapHeight) {
  _mapManager.uploadNewMap(map, mapWidth, mapHeight);
}

int SceneManager::getMapHeight() const { return _mapManager.mapHeight(); }
int SceneManager::getMapWidth() const { return _mapManager.mapWidth(); }

bool SceneManager::isMapAvailable() const {
  return _mapManager.isMapAvailable();
}

void SceneManager::uploadTextureForWall(const char &mapChar,
                                        std::string &wallTex) {
  _mapManager.uploadWallTextureFor(mapChar, wallTex);
}

const std::string &SceneManager::getWallTextureForMapChar(const char &mapChar) {
  return _mapManager.getWallTexForMapChar(mapChar);
}

// Entity Related functions
void SceneManager::AddEntity(Entity &entity) {
  _entityManager.addEntity(entity);
}
void SceneManager::removeEntity(Entity &entity) {
  _entityManager.removeEntity(entity.ID());
}
void SceneManager::removeEntity(int entityId) {
  _entityManager.removeEntity(entityId);
}
void SceneManager::removeAllEntities() { _entityManager.removeAllEntities(); }

Tile SceneManager::isOccupied(int x, int y) const {
  if (_entityManager.getEntityIdAtPos(x, y) != -1)
    return Tile::ENTITY;

  if (_mapManager.isWall(x, y) == true)
    return Tile::WALL;

  return Tile::NONE;
}

// Player related functions
void SceneManager::setPlayerX(int new_x) { _player.set_x(new_x); }
void SceneManager::setPlayerY(int new_y) { _player.set_y(new_y); }
Player &SceneManager::getPlayerRef() { return _player; }

// Other
void SceneManager::loadResources(
    const std::string &filePath,
    std::unordered_map<std::string, std::string> &outTextures) {

  std::stringstream temp;
  std::string value;
  std::string key;
  for (const auto &entry : std::filesystem::directory_iterator(filePath)) {
    if (std::filesystem::is_regular_file(entry)) {
      std::ifstream current_file(entry.path());
      if (current_file.fail()) {
        std::cerr << "ERROR: failed to open file: " << entry.path().string()
                  << " reading next texture file..." << std::endl;
        continue;
      }

      temp.str(std::string());
      temp << current_file.rdbuf();
      key = entry.path().stem().string();
      if (outTextures.find(key) != outTextures.end()) {
        std::cerr << "ERROR: texture for this type of tile is already present. "
                     "Skipping this one...."
                  << std::endl;
        std::cerr << "texture: " << std::endl;
        std::cout << temp.str() << std::endl;
        continue;
      }
      value = temp.str();
      outTextures[key] = value;
      current_file.close();
    }
  }
}
