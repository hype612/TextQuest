#include "../Headers/TextureRequestQueue.h"

TextureRequestQueue::TextureRequestQueue() {}
TextureRequestQueue::~TextureRequestQueue() { _mtx.unlock(); } // just to make sure;

void TextureRequestQueue::push(int entityId)
{
  _mtx.lock();
  _texQueue.push(entityId);
  _mtx.unlock();
}


void TextureRequestQueue::pop()
{
  _mtx.lock();
  _texQueue.pop();
  _mtx.unlock();
}

