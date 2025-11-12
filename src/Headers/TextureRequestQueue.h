#ifndef TEXTUREREQUESTQUEUE_H
#define TEXTUREREQUESTQUEUE_H

#include <algorithm>
#include <deque>
#include <mutex>
#include <shared_mutex>
#include <tuple>

class TextureRequestQueue {
public:
  TextureRequestQueue();
  ~TextureRequestQueue();

  void push(std::tuple<int, int> position);
  bool contains(const std::tuple<int, int> &position) const;
  void pop();
  const std::tuple<int, int> &top() const;

private:
  std::deque<std::tuple<int, int>> _texQueue;
  mutable std::shared_mutex _mtx;
};

#endif // TEXTUREREQUESTQUEUE_H
