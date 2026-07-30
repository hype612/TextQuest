#ifndef ENTITYDISTANCE_H
#define ENTITYDISTANCE_H

using EntityId = int;

struct EntityDistance {
  EntityId id;
  float distance;

  bool operator<(const EntityDistance &rval) const {
    return distance < rval.distance;
  }
};
#endif // ENTITYDISTANCE_H
