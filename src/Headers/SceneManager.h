#ifndef SCENEHANDLER_H
#define SCENEHANDLER_H

#include "Object.h"
#include "Entity.h"
#include "EntityManager"
#include "MapManager"
#include <unordered_map>
#include <tuple>
#include <vector>
#include <string>

typedef std::tuple<int, int> coord_t;

struct coord_hash
{
    std::size_t operator()(const coord_t& k) const
    {
        return std::get<0>(k) ^ std::get<1>(k);
    }
};


class SceneManager
{
public:
    SceneManager();
    SceneManager(std::wstring& map); // pass a map and construct a scene from that


    void process();
    // i do not want to give access to entities and objects
    // for now i wont implement these
    //Entity EntityAt(int x, int y) const;
    //Object ObjectAt(int x, int y);
    bool isOccupied(int x, int y) const;

    // Map Related functions
    void initializeNewMap(std::wstring& map, int mapWidth, int mapHeight);

    // Entity Related functions
    void AddEntity(Entity& entity);
    void removeEntity(Entity& entity);
    void removeEntity(int entityId);
    void removeAllEntities();
    
private:
    //std::unordered_map<coord_t, int, coord_hash> _entityPositionMap; // stores entity._id, indexes with coordinates
    MapManager    _mapManager; 
    EntityManager _entityManager;
};



#endif // SCENEHANDLER_H
