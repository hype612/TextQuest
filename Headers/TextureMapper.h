#ifndef TEXTUREMAPPER_H
#define TEXTUREMAPPER_H

#include <unordered_map>
#include <string>

namespace TextureMapper {
	extern std::unordered_map<wchar_t, int> accumulator;
	// currently returns an array of chars which should represent a column
	extern std::wstring currentTextureMipMap;
	extern int stepper;
	extern int tex_height;
	extern int tex_width;

	float estimateHeight(float distance);
	float estimateWidth(float distance);
	void setCurrentTexture(float distance, const std::string& mode, std::wstring* tex);
	void verticalDownscale(int height, int width);
	void horizontalDownscale(int height, int width);
	void repeatingHorizontalUpscale(int height, int width);
	void repeatingVerticalUpscale(int height, int width);
	void createRepeatingTexture(int height, int width, std::wstring* tex);
	void createDownScaledTexture(int height, int width, std::wstring* tex);
	std::wstring CreateMipMapAtLevel(float distance, const std::string& mode, std::wstring* tex);
	
	std::wstring GetCharColumnAtPosition(int height);

	std::wstring ScaleToHeight(int height, std::wstring column);

}

#endif