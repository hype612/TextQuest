#ifndef TEXTUREREQUESTQUEUE_H
#define TEXTUREREQUESTQUEUE_H

#include "TextureRequest.h"
#include <deque>

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
