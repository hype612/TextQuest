#include "../Headers/TextureMapper.h"

TextureMapper::TextureMapper(std::wstring initTexture) : _textureMipMap(initTexture)
{
	GenerateTextureMask();
} 


float TextureMapper::estimateHeight(float distance)
{
	if (distance > 9.f)
		return 0;
	return (-(distance / 0.105f) + 98.2f);
}
float TextureMapper::estimateWidth(float distance)
{
	if (distance > 9.f)
		return 0;
	return (-(distance / 0.0375f) + 294.f);
}

// naive mask generator
// technically works, but later when its 
// shading time, the most bright pixels wont be shaded
// if the shading is dependant of the mask
void TextureMapper::GenerateTextureMask()
{
	int tx_width = _textureMipMap.find(L'\n');
	int tx_height = tx_width;
	std::vector<int> ret(tx_height, tx_width);
	for(int i = 0; i < tx_height; i++)
	{
		for(int j = 0; j < tx_width; j++)
		{
			if(_textureMipMap[i * tx_width + i + j] == ' ')
				ret[i,j] = 0;
			else
				ret[i,j] = 1;
		}
	}
}

void TextureMapper::setCurrentTexture(float distance, const std::string& mode, std::wstring* tex)
{
	_stepper = 0;
	int height = (int)(estimateHeight(distance));
	int width = (int)(estimateWidth(distance));
		
	std::wstring line = *tex;
	_tex_width = line.find(L'\n');
	
	_tex_height = _tex_width;
	_textureMipMap = *tex;
	// HORIZONTAL SCALE
	if (_tex_width < width)
	{
		nxInterpolationUpscale(width);
	}
	if (_tex_width > width)
	{
		nxInterpolationDownscale(width);
	}
	// VERTICAL SCALE
	if (_tex_height < height)
	{
		nyInterpolationUpscale(height);
	}
	if (_tex_height > height)
	{
		nyInterpolationDownscale(height);
	}
	_textureMipMap;
}

void TextureMapper::repeatingHorizontalDownscale(int height, int width)
{
	std::wstring tex = L"";
	tex.reserve(_tex_height * (width + 1));
	if (_tex_width > width)
	{
		for (int i = 0; i < _tex_height; i++)
		{
			tex.append(_textureMipMap.substr(i * _tex_width + i, width));
			tex.push_back(L'\n');
		}
	}
	_tex_width = width;
	_textureMipMap = std::move(tex);
}

void TextureMapper::repeatingVerticalDownscale(int height, int width)
{
	_textureMipMap = _textureMipMap.substr(0, width * height + height);
	_tex_height = height;
}


void TextureMapper::repeatingHorizontalUpscale(int height, int width)
{
	std::wstring line = L"";
	line.reserve(width);
	std::wstring new_tex = L"";
	new_tex.reserve(height * width);
	
	for (int i = 0; i < _tex_height; i++)
	{
		line = _textureMipMap.substr((1 + _tex_width) * i, _tex_width);
		while (line.length() < width)
			line.append(line.substr(0, width - line.length()));
		new_tex.append(line);
		new_tex.push_back(L'\n');
	}
	_textureMipMap = std::move(new_tex);
	_tex_width = width;
}

void TextureMapper::repeatingVerticalUpscale(int height, int width)
{
	std::wstring new_tex = _textureMipMap;
	int i = 0;
	while (_tex_height < height)
	{
		new_tex += new_tex.substr((_tex_width + 1) * i, _tex_width);
		i = (i + 1) % _tex_height;
		_tex_height++;
	}
	_textureMipMap = std::move(new_tex);
}


std::wstring TextureMapper::GetCharColumnAtPosition(int height) 
{
	if (_tex_height < 1 || _tex_width < 1)
	{
		return std::wstring(L"");
	}
	if (_stepper >= _tex_width)
	{
		_stepper = 0;
	}
	std::wstring ret = L"";
	ret.reserve(height);

	for (int i = 0; i < _tex_height; i++)
	{
		ret.push_back(_textureMipMap[(i % _tex_height) * _tex_width + _stepper]);
	}
	int j = 0;
	while (ret.length() < height)
	{
		ret.push_back(_textureMipMap[(j % _tex_height) * _tex_width + _stepper]);
		j++;
	}
	_stepper++;
	return ret;
}
// =======================
//  interpolation scaling
// =======================
void TextureMapper::nxInterpolationDownscale(int width)
{
	std::wstring new_tex;
	new_tex.reserve(width * _tex_height);

	float x_ratio = static_cast<float>(_tex_width) / width;

	for( int y = 0; y < _tex_height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int src_x = static_cast<int>(x * x_ratio);
			wchar_t pb = _textureMipMap[y * _tex_width + src_x + y];
			if (pb != L'\n')
				new_tex.push_back(pb);
			else
				new_tex.push_back(_textureMipMap[y * _tex_width + src_x - 1]);
		}
		new_tex.push_back(L'\n');
	}
	_textureMipMap = std::move(new_tex);
	_tex_width = width;
}

void TextureMapper::nxInterpolationUpscale(int width)
{
	std::wstring new_tex;
	new_tex.reserve(width * _tex_height);

	float x_ratio = static_cast<float>(_tex_width) / width;

	for( int y = 0; y < _tex_height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int src_x = static_cast<int>(x * x_ratio);
			wchar_t pb = _textureMipMap[y * _tex_width + src_x + y];
			if (pb != L'\n')
				new_tex.push_back(pb);
			else
				new_tex.push_back(_textureMipMap[y * _tex_width + src_x - 1]);
		}
		new_tex.push_back(L'\n');
	}
	_textureMipMap = std::move(new_tex);
	_tex_width = width;
}

void TextureMapper::nyInterpolationDownscale(int height)
{
	std::wstring new_tex;
	new_tex.reserve(_tex_width * height);

	float y_ratio = static_cast<float>(_tex_height) / height;

	for( int y = 0; y < height; y++)
	{
		for (int x = 0; x < _tex_width; x++)
		{
			int src_y = static_cast<int>(y * y_ratio);
			wchar_t pb = _textureMipMap[src_y * _tex_width + x + src_y];
			if (pb != L'\n')
				new_tex.push_back(pb);
			else
				new_tex.push_back(_textureMipMap[src_y * _tex_width + x - 1]);
		}
		new_tex.push_back(L'\n');
	}
	_textureMipMap = std::move(new_tex);
	_tex_height = height;
}

void TextureMapper::nyInterpolationUpscale(int height)
{
	std::wstring new_tex;
	new_tex.reserve(_tex_width * height);

	float y_ratio = static_cast<float>(_tex_height) / height;

	for( int y = 0; y < height; y++)
	{
		for (int x = 0; x < _tex_width; x++)
		{
			int src_y = static_cast<int>(y * y_ratio);
			wchar_t pb = _textureMipMap[src_y * _tex_width + x + src_y];
			if (pb != L'\n')
				new_tex.push_back(pb);
			else
				new_tex.push_back(_textureMipMap[src_y * _tex_width + x - 1]);
		}
		new_tex.push_back(L'\n');
	}
	_textureMipMap = std::move(new_tex);
	_tex_height = height;
}




std::wstring TextureMapper::ScaleToHeight(int height,  std::wstring column) {
	std::wstring r_column = L"";
	r_column.reserve(height);
	while (r_column.length() < height) {
		for (wchar_t c : column) {
			r_column.push_back(c);
		}
	}
	return r_column.substr(0, height);
}
