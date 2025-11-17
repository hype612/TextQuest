#ifndef TEXTUREREQUESTQUEUE_H
#define TEXTUREREQUESTQUEUE_H

#include "./Tile.h"
#include <algorithm>
#include <deque>
#include <mutex>
#include <shared_mutex>
#include <tuple>

class TextureRequestQueue {
public:
  TextureRequestQueue();
  ~TextureRequestQueue();

  void push(std::tuple<int, int, Tile> input);
  bool contains(const std::tuple<int, int> &position) const;
  void pop();
  const std::tuple<int, int, Tile> &top() const;
  bool isEmpty() const;

private:
  std::deque<std::tuple<int, int, Tile>> _texQueue;
  mutable std::shared_mutex _mtx;
};

#endif // TEXTUREREQUESTQUEUE_H
