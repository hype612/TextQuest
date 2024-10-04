#ifndef TEXTUREMAPPER_H
#define TEXTUREMAPPER_H

#include <unordered_map>
#include <string>

namespace TextureMapper {
	extern std::unordered_map<wchar_t, int> accumulator;
	// currently returns an array of chars which should represent a column
	std::wstring GetCharColumnAtPosition(int height, wchar_t& type, std::wstring& texture);

	std::wstring ScaleToHeight(int height, wchar_t& type, std::wstring& texture);
}

#endif