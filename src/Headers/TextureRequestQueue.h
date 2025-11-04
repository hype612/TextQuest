#ifndef TEXTUREREQUESTQUEUE_H
#define TEXTUREREQUESTQUEUE_H

#include <mutex>
#include <queue>

class TextureRequestQueue {
public:
  TextureRequestQueue();
  ~TextureRequestQueue();

  void push(int entityId);
  void pop();
private:
  std::queue<int> _texQueue;
  std::mutex _mtx;
};


#endif // TEXTUREREQUESTQUEUE_H
