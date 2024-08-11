#include "Headers\TextureLoader.h"

std::unordered_map<wchar_t, std::wstring> Texture_loader::load_textures(std::string path) {
	std::wstringstream temp;
	std::unordered_map<wchar_t, std::wstring> returned_textures;
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (std::filesystem::is_regular_file(entry)) {
			std::wifstream current_file(entry.path());
			temp.str(std::wstring());
			//current_file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
			temp << current_file.rdbuf();
			// create the entry to the map
			if (entry.path().string().find("brickwall") != std::string::npos) {
				returned_textures[L'#'] = temp.str();
			}
			if (entry.path().string().find("skeleton") != std::string::npos) {
				returned_textures[L'W'] = temp.str();
			}
			if (entry.path().string().find("slime") != std::string::npos) {
				returned_textures[L'S'] = temp.str();
			}
			current_file.close();
		}
	}
	return returned_textures;
}
