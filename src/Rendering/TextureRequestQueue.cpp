#include "../Headers/TextureRequestQueue.h"
#include <shared_mutex>

TextureRequestQueue::TextureRequestQueue() {}
TextureRequestQueue::~TextureRequestQueue() {
  _mtx.unlock();
} // just to make sure;

void TextureRequestQueue::push(std::tuple<int, int> position) {
  std::unique_lock<std::shared_mutex> lock(_mtx);
  _texQueue.push_back(position);
}

bool TextureRequestQueue::contains(const std::tuple<int, int> &position) const {

  std::shared_lock<std::shared_mutex> lock(_mtx);
  if (std::find(_texQueue.begin(), _texQueue.end(), position) !=
      _texQueue.end())
    return true;
  else
    return false;
}

void TextureRequestQueue::pop() {
  std::unique_lock<std::shared_mutex> lock(_mtx);
  _texQueue.pop_front();
}

const std::tuple<int, int> &TextureRequestQueue::top() const {
  std::shared_lock<std::shared_mutex> lock(_mtx);
  return _texQueue.front();
}
