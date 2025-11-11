#include "../Headers/TextureRequestQueue.h"

TextureRequestQueue::TextureRequestQueue() {}
TextureRequestQueue::~TextureRequestQueue() { _mtx.unlock(); } // just to make sure;

void TextureRequestQueue::push(std::tuple<int, int> position)
{
  _mtx.lock();
  _texQueue.push_ back(position);
  _mtx.unlock();
}


void contains(std::tuple<int, int> position)
{
  if( std::find(_texQueue.begin(), _texQueue.end(), position) != _texQueue.end())
    return true;
  else
    return false;
}


void TextureRequestQueue::pop()
{
  _mtx.lock();
  _texQueue.pop_front();
  _mtx.unlock();
}

