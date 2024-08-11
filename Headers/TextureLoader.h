#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <filesystem>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <iostream>

namespace Texture_loader {
	std::unordered_map<wchar_t, std::wstring> load_textures(std::string path);
}

#endif