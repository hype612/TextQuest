#include "../Headers/TextureRequestQueue.h"

void TextureRequestQueue::push(TextureRequest input) {
  _texQueue.push_back(input);
}

TextureRequest TextureRequestQueue::pop() {
  if (_texQueue.empty()) {
    return TextureRequest{-1, -1, -1, -1, -1, -1.f, -1.f};
  }
  TextureRequest topEl = _texQueue.back();
  _texQueue.pop_back();
  return topEl;
}

bool TextureRequestQueue::isEmpty() const { return _texQueue.empty(); }
