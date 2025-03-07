#include "Headers\TextureLoader.h"

// for clarity: the key will become the first character of the texture
// thus the first character of the texture MUST be unique AND match the 
// character which is assigned to the texture on the map.
// currently the first character is copied from the second, for later visual clarity

/*

std::unordered_map<wchar_t, std::wstring> Texture_loader::load_textures(std::string& path) {
	std::wstringstream temp;
	std::wstring value;
	wchar_t key;
	std::unordered_map<wchar_t, std::wstring> returned_textures;
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (std::filesystem::is_regular_file(entry)) {
			std::wifstream current_file(entry.path());
			if (current_file.fail()) {
				std::cerr << "ERROR: failed to open file: " << entry.path().string() << " reading next texture file..." << std::endl;
				continue;
			}
			
			temp.str(std::wstring());
			temp << current_file.rdbuf();
			key = temp.str()[0];
			if (returned_textures.find(key) != returned_textures.end())
			{
				std::cerr << "ERROR: texture for this type of tile is already present. Skipping this one...." << std::endl;
				std::cerr << "texture: " << std::endl;
				std::wcout << temp.str() << std::endl;
				continue;
			}
			value = temp.str();
			//value[0] = value[1];
			// create the entry to the map
			if (entry.path().string().find("wall") != std::string::npos) {
				returned_textures[key] = value;
			}
			if (entry.path().string().find("skeleton") != std::string::npos) {
				returned_textures[key] = value;
			}
			if (entry.path().string().find("slime") != std::string::npos) {
				returned_textures[key] = value;
			}
			current_file.close();
		}
	}
	return returned_textures;
}
*/


std::unordered_map<std::pair<wchar_t, int>, std::wstring> Texture_loader::load_textures(std::string& path) {
	std::wstringstream temp;
	std::wstring value;
	wchar_t key;
	std::unordered_map<std::pair<wchar_t, int>, std::wstring> returned_textures;
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (std::filesystem::is_regular_file(entry)) {
			std::wifstream current_file(entry.path());
			if (current_file.fail()) {
				std::cerr << "ERROR: failed to open file: " << entry.path().string() << " reading next texture file..." << std::endl;
				continue;
			}
			
			temp.str(std::wstring());
			temp << current_file.rdbuf();
			key = temp.str()[0];
			// if lv1 mipmap exists, then all of them exist	
			if (returned_textures.find(std::pair(key,1)) != returned_textures.end())
			{
				std::cerr << "ERROR: texture for this type of tile is already present. Skipping this one...." << std::endl;
				std::cerr << "texture: " << std::endl;
				std::wcout << temp.str() << std::endl;
				continue;
			}
			value = temp.str();
			//value[0] = value[1];
			// create the entry to the map
			if (entry.path().string().find("wall") != std::string::npos) {
				//returned_textures[key] = value;
				std::unordered_map<std::pair<wchar_t, int>, std::wstring> temp = Texture_loader::generate_MipMapLevels(value, key);
				returned_textures.insert(temp.begin(), temp.end());
			}
			if (entry.path().string().find("skeleton") != std::string::npos) {
				//returned_textures[key] = value;
				std::unordered_map<std::pair<wchar_t, int>, std::wstring> temp = Texture_loader::generate_MipMapLevels(value, key);
				returned_textures.insert(temp.begin(), temp.end());
			}
			if (entry.path().string().find("slime") != std::string::npos) {
				//returned_textures[key] = value;
				std::unordered_map<std::pair<wchar_t, int>, std::wstring> temp = Texture_loader::generate_MipMapLevels(value, key);
				returned_textures.insert(temp.begin(), temp.end());
			}
			current_file.close();
		}
	}
	return returned_textures;
}


std::unordered_map<std::pair<wchar_t, int>, std::wstring> Texture_loader::generate_MipMapLevels(std::wstring& tex, wchar_t tex_key)
{
	std::unordered_map<std::pair<wchar_t, int>, std::wstring> mipmaps;
	for (int i = 1; i <= 8; i++)
	{
		mipmaps[std::pair(tex_key, i)] = TextureMapper::CreateMipMapAtLevel(i, "repeat", &tex);
	}
	return mipmaps;
}
