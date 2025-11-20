#include "../Headers/TextureRequestQueue.h"
#include <mutex>

TextureRequestQueue::TextureRequestQueue() {}

void TextureRequestQueue::push(std::tuple<int, int, Tile, float> input) {
  {
    std::unique_lock<std::shared_mutex> lock(_mtx);
    _texQueue.push_back(input);
  }
  _cv.notify_one();
}

std::tuple<int, int, Tile, float> TextureRequestQueue::pop() {
  std::unique_lock<std::mutex> cvLock(_cvMtx);
  _cv.wait(cvLock, [this] {
    std::shared_lock<std::shared_mutex> lock(_mtx);
    return !_texQueue.empty() || _rayCompleted;
  });
  std::unique_lock<std::shared_mutex> lock(_mtx);
  if (_texQueue.empty()) {
    return std::tuple<int, int, Tile, float>(-1, -1, Tile::NONE, -1.f);
  }
  std::tuple<int, int, Tile, float> topEl = _texQueue.front();
  _texQueue.pop_front();
  return topEl;
}

bool TextureRequestQueue::isEmpty() const {
  std::shared_lock<std::shared_mutex> lock(_mtx);
  if (_texQueue.empty()) {
    return true;
  } else {
    return false;
  }
}

void TextureRequestQueue::setRayCompleted(bool done) {
  {
    std::unique_lock<std::mutex> cvLock(_cvMtx);
    _rayCompleted = done;
  }
  _cv.notify_all();
}

bool TextureRequestQueue::isRayCompleted() {
  std::unique_lock<std::mutex> lock(_cvMtx);
  return _rayCompleted;
}

void TextureRequestQueue::setTexturesReady(bool done) {
  {
    std::unique_lock<std::mutex> cvLock(_cvMtx);
    _texturesReady = done;
  }
  _cv.notify_all();
}

bool TextureRequestQueue::areTexturesReady() {
  std::unique_lock<std::mutex> lock(_cvMtx);
  return _texturesReady;
}

void TextureRequestQueue::waitForTextures() {
  std::unique_lock<std::mutex> lock(_cvMtx);
  _cv.wait(lock, [this] { return _texturesReady; });
}
