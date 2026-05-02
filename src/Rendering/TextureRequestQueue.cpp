#include "../Headers/TextureRequestQueue.h"

void TextureRequestQueue::push(TextureRequest input) {
  _texQueue.push_back(input);
}

TextureRequest TextureRequestQueue::pop() {
  if (_texQueue.empty()) {
    return TextureRequest{-1, -1, Tile::NONE, -1, -1.f, -1.f};
  }
  TextureRequest topEl = _texQueue.front();
  _texQueue.pop_front();
  return topEl;
}

bool TextureRequestQueue::isEmpty() const { return _texQueue.empty(); }

/*
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
}*/
