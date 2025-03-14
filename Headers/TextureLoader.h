#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <filesystem>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <iostream>
#include <utility>
#include "TextureMapper.h"


namespace Texture_loader {
	std::unordered_map<wchar_t, std::wstring> load_textures(std::string& path);
	//std::unordered_map<std::pair<wchar_t,int>, std::wstring> load_textures(std::string& path);
	//std::unordered_map<std::pair<wchar_t,int>, std::wstring> generate_MipMapLevels(std::wstring&, wchar_t tex_key);
}

#endif