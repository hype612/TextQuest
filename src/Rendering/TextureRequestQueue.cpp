#include "../Headers/TextureRequestQueue.h"
#include <shared_mutex>

TextureRequestQueue::TextureRequestQueue() {}
TextureRequestQueue::~TextureRequestQueue() {
  _mtx.unlock();
} // just to make sure;

void TextureRequestQueue::push(std::tuple<int, int, Tile> input) {
  std::unique_lock<std::shared_mutex> lock(_mtx);
  _texQueue.push_back(input);
}

bool TextureRequestQueue::contains(const std::tuple<int, int> &position) const {

  std::shared_lock<std::shared_mutex> lock(_mtx);
  std::tuple<int, int, Tile> withEntity(std::get<0>(position),
                                        std::get<1>(position), Tile::ENTITY);
  std::tuple<int, int, Tile> withWall(std::get<0>(position),
                                      std::get<1>(position), Tile::WALL);
  if (std::find(_texQueue.begin(), _texQueue.end(), withEntity) !=
      _texQueue.end())
    return true;
  if (std::find(_texQueue.begin(), _texQueue.end(), withWall) !=
      _texQueue.end())
    return true;
  else
    return false;
}

void TextureRequestQueue::pop() {
  std::unique_lock<std::shared_mutex> lock(_mtx);
  _texQueue.pop_front();
}

const std::tuple<int, int, Tile> &TextureRequestQueue::top() const {
  std::shared_lock<std::shared_mutex> lock(_mtx);
  return _texQueue.front();
}

bool TextureRequestQueue::isEmpty() const {
  if (_texQueue.empty()) {
    return true;
  } else {
    return false;
  }
}
