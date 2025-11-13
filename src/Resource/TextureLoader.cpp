#include "../Headers/TextureLoader.h"

std::unordered_map<wchar_t, std::wstring>
Texture_loader::load_textures(std::string &path) {
  std::wstringstream temp;
  std::wstring value;
  wchar_t key;
  std::unordered_map<wchar_t, std::wstring> returned_textures;
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (std::filesystem::is_regular_file(entry)) {
      std::wifstream current_file(entry.path());
      if (current_file.fail()) {
        std::cerr << "ERROR: failed to open file: " << entry.path().string()
                  << " reading next texture file..." << std::endl;
        continue;
      }

      temp.str(std::wstring());
      temp << current_file.rdbuf();
      key = temp.str()[0];
      if (returned_textures.find(key) != returned_textures.end()) {
        std::cerr << "ERROR: texture for this type of tile is already present. "
                     "Skipping this one...."
                  << std::endl;
        std::cerr << "texture: " << std::endl;
        std::wcout << temp.str() << std::endl;
        continue;
      }
      value = temp.str();
      // value[0] = value[1];
      //  create the entry to the map
      if (entry.path().string().find("wall") != std::string::npos) {
        returned_textures[key] = value;
      }
      if (entry.path().string().find("enemy") != std::string::npos) {
        returned_textures[key] = value;
      }
      current_file.close();
    }
  }
  return returned_textures;
}
