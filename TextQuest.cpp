#include "src/Headers/Core.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

int main() {
  GameEngine instance(240, 80,
                      ".\\Resources\\Textures"); // must give res to screen
  instance.run_game();
  /*
  std::unordered_map<wchar_t, std::wstring> test_texts =
  Texture_loader::load_textures(".\\Resources\\Textures"); wchar_t pls_work_ptr
  = L'#'; std::wstring test_col = TextureMapper::GetCharColumnAtPosition(70,
  pls_work_ptr, test_texts['#']); std::wcout << test_col << std::endl;*/
  return 0;
}
