#include "Headers\TextureMapper.h"

std::unordered_map<wchar_t, int> TextureMapper::accumulator;

std::wstring TextureMapper::GetCharColumnAtPosition(int height, wchar_t& type, std::wstring& texture) {
	if (accumulator.find(type) == accumulator.end()) {
		accumulator[type] = 0;
	}
	else {
		accumulator[type] += 1;
	}



	if (height <= 32) {
		return texture.substr((accumulator[type] % 32), height);
	}
	else if (height >= 32) {
		// scale up
		std::wstring ret_val = texture.substr((accumulator[type] % 32), 32) + texture.substr((accumulator[type] % 32), 32) + texture.substr((accumulator[type] % 32), 32);
		return ret_val.substr(0, height);
	}
}

std::wstring TextureMapper::ScaleToHeight(int height, wchar_t& type,  std::wstring& texture) {
	
	// quick dumb magic for testing
	if (height <= 32) {
		return texture.substr((accumulator[type] % 32), height);
	}
	else if (height >= 32) {
		// scale up
		std::wstring ret_val = texture.substr((accumulator[type] % 32), 32) + texture.substr((accumulator[type] % 32), 32);
		return ret_val.substr(0, height);

	}
}
