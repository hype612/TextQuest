#ifndef TEXTUREREQUESTQUEUE_H
#define TEXTUREREQUESTQUEUE_H

#include "./Tile.h"
#include "TextureRequest.h"
#include <algorithm>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <shared_mutex>
#include <tuple>

class TextureRequestQueue {
public:
  TextureRequestQueue() = default;
  ~TextureRequestQueue() = default;

  void push(TextureRequest input);
  TextureRequest pop();
  bool isEmpty() const;

private:
  std::deque<TextureRequest> _texQueue;
};

#endif // TEXTUREREQUESTQUEUE_H
