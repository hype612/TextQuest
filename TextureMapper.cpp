#include "Headers\TextureMapper.h"
#include <iostream>

std::unordered_map<wchar_t, int> TextureMapper::accumulator;

std::wstring TextureMapper::GetCharColumnAtPosition(int height, wchar_t& type, std::wstring& texture) {
	if (accumulator.find(type) == accumulator.end()) {
		accumulator[type] = 0;
	}
	else {
		accumulator[type] += 1;
		accumulator[type] = accumulator[type] % 32;
	}
	
	std::wstring ret_value;
	ret_value.reserve(32);
	for(int i = 0; i < 32; i++) {
		ret_value += texture[i * 32 + accumulator[type] + i]; // +i for every \n at the end of every line
	}

	if (height <= 32) {
		return ret_value.substr(0, height);
	}
	else if (height >= 32) {
		// scale up
		while (ret_value.length() < height)
			ret_value += ret_value;
		return ret_value.substr(0, height);
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
