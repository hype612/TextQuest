#ifndef SCENEHANDLER_H
#define SCENEHANDLER_H

#include "Object.h"
#include "Entity.h"
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


class SceneHandler
{
public:
    SceneHandler();
    SceneHandler(std::wstring& map); // pass a map and construct a scene from that

    // i do not want to give access to entities and objects
    // for now i wont implement these
    //Entity EntityAt(int x, int y) const;
    //Object ObjectAt(int x, int y);
    bool IsOccupied(int x, int y) const;

private:
    std::vector<Entity> _entityContainer; // indexed with entity._id
    //std::unordered_map<coord_t, int, coord_hash> _entityPositionMap; // stores entity._id, indexes with coordinates
    std::unordered_map<coord_t, Object, coord_hash> _objectContainer; // indexed with coordinates
    std::wstring _map;
    
};



#endif // SCENEHANDLER_H
