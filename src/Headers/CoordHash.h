#ifndef COORDHASH_H
#define COORDHASH_H

#include <functional>
#include <tuple>

typedef std::tuple<int, int> coord_t;

struct coord_hash {
  std::size_t operator()(const coord_t &k) const {
    return std::get<0>(k) ^ std::get<1>(k);
  }
};

#endif // COORDHASH_H
