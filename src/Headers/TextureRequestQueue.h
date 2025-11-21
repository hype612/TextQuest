#ifndef TEXTUREREQUESTQUEUE_H
#define TEXTUREREQUESTQUEUE_H

#include "./Tile.h"
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

  void push(std::tuple<int, int, Tile, float> input);
  std::tuple<int, int, Tile, float> pop();
  bool isEmpty() const;

  void setRayCompleted(bool done);
  bool isRayCompleted();

  void setTexturesReady(bool done);
  bool areTexturesReady();
  void waitForTextures();

private:
  std::deque<std::tuple<int, int, Tile, float>> _texQueue;
  mutable std::shared_mutex _mtx;
  mutable std::mutex _cvMtx;
  std::condition_variable_any _cv;
  bool _rayCompleted = false;
  bool _texturesReady = false;
};

#endif // TEXTUREREQUESTQUEUE_H
