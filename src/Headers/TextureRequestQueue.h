#ifndef TEXTUREREQUESTQUEUE_H
#define TEXTUREREQUESTQUEUE_H

#include <mutex>
#include <deque>
#include <tuple>
#include <algorithm>

class TextureRequestQueue {
public:
  TextureRequestQueue();
  ~TextureRequestQueue();

  void push(std::tuple<int, int> position);
  void contains(std::tuple<int, int> position);
  void pop();
private:
  std::deque<std::tuple<int, int>> _texQueue;
  std::mutex _mtx;
};


#endif // TEXTUREREQUESTQUEUE_H
