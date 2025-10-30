#ifndef TEXTUREMAPPER_H
#define TEXTUREMAPPER_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>

class TextureMapper
{
public:
	float estimateHeight(float distance);
	float estimateWidth(float distance);
	void setCurrentTexture(float distance, const std::string& mode, std::wstring* tex);
  void rescaleCurrentTexture(float distance);	
	// =============================	
	// repeating scaling
	// =============================	
	
	void repeatingVerticalDownscale(int height, int width);
	void repeatingHorizontalDownscale(int height, int width);
	void repeatingHorizontalUpscale(int height, int width);
	void repeatingVerticalUpscale(int height, int width);
	
	// =============================	
	//   interpolation scaling(nn)
	// =============================	
	
	void nxInterpolationDownscale(int width);
	void nxInterpolationUpscale(int width);
	void nyInterpolationDownscale(int height);
	void nyInterpolationUpscale(int height);
	
	
	// =========================	
	//   return of textures 
	// =========================	
  std::wstring getNextTexColumn(int height); 
  std::wstring getMaskColumn(int height); 
  std::wstring getTexture();
  std::wstring getMask(); 

	TextureMapper(std::wstring initTexture);
private:
	std::wstring _textureMipMap = L"";
	std::wstring _textureMask =  L""; 
	int _stepper = -1;
  int _tex_height = -1;
  int _tex_width = -1;

	void GenerateTextureMask();	
	std::wstring ScaleToHeight(int height, std::wstring column);
	TextureMapper(); // default construction should not be allowed
};


#endif
