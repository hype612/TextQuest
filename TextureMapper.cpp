#include "Headers\TextureMapper.h"
#include <iostream>

std::unordered_map<wchar_t, int> TextureMapper::accumulator;

std::wstring TextureMapper::currentTextureMipMap = L"";
int TextureMapper::stepper = -1;
int TextureMapper::tex_height = -1;
int TextureMapper::tex_width = -1;


float TextureMapper::estimateHeight(float distance)
{
	if (distance > 8.5f)
		return 0;
	return (-(distance / 0.105f) + 98.2f);
}
float TextureMapper::estimateWidth(float distance)
{
	if (distance > 8.5f)
		return 0;
	return (-(distance / 0.0375f) + 294.f);
}

std::wstring TextureMapper::CreateMipMapAtLevel(float distance, const std::string& mode, std::wstring* tex)
{
	std::wstring rmipmap = L"";
	int height = (int)(TextureMapper::estimateHeight(distance));
	int width = (int)(TextureMapper::estimateWidth(distance));
	int tx_w = 0;
	int tx_h = 0;
	std::wstring line = *tex;
	tx_w = line.find(L'\n');
	
	tx_h = tx_w;
	rmipmap = *tex;
	// HORIZONTAL SCALE
	if (tx_w < width)
	{
		std::wstring line = L"";
		line.reserve(width);
		std::wstring new_tex = L"";
		new_tex.reserve(height * width);
		
		for (int i = 0; i < tx_h; i++)
		{
			line = rmipmap.substr((1 + tx_w) * i, tx_w);
			while (line.length() < width)
				line.append(line.substr(0, width - line.length()));
			new_tex.append(line);
			new_tex.push_back(L'\n');
		}
		rmipmap = std::move(new_tex);
	}
	if (tx_w > width)
	{
		std::wstring tex = L"";
		tex.reserve(tx_h * (width + 1));
		if (tx_w > width)
		{
			for (int i = 0; i < tx_h; i++)
			{
				tex.append(currentTextureMipMap.substr(i * tex_width + i, width));
				tex.push_back(L'\n');
			}
		}
		rmipmap = std::move(tex);
	}
	// VERTICAL SCALE
	if (tx_h < height)
	{
		std::wstring new_tex = rmipmap;
		int i = 0;
		while (tx_h < height)
		{
			new_tex += new_tex.substr((tx_w + 1) * i, tx_w);
			i = (i + 1) % tx_h;
			tx_h++;
		}
		rmipmap = std::move(new_tex);
	}
	if (tx_h > height)
	{
		rmipmap = rmipmap.substr(0, width * height + height);
		tx_h = height;
	}
	return rmipmap;
}

void TextureMapper::setCurrentTexture(float distance, const std::string& mode, std::wstring* tex)
{
	stepper = 0;
	int height = (int)(TextureMapper::estimateHeight(distance));
	int width = (int)(TextureMapper::estimateWidth(distance));
		
	std::wstring line = *tex;
	tex_width = line.find(L'\n');
	
	tex_height = tex_width;
	TextureMapper::currentTextureMipMap = *tex;
	// HORIZONTAL SCALE
	if (tex_width < width)
	{
		TextureMapper::repeatingHorizontalUpscale(height, width);
	}
	if (tex_width > width)
	{
		TextureMapper::horizontalDownscale(height, width);
	}
	// VERTICAL SCALE
	if (tex_height < height)
	{
		TextureMapper::repeatingVerticalUpscale(height, width);
	}
	if (tex_height > height)
	{
		TextureMapper::verticalDownscale(height, width);
	}
	/*
	// rare edge case
	if (tex_height == height && tex_width == width)
	{
		currentTextureMipMap = *tex;
		return;
	}
	// downscale
	if (tex_height < height && tex_width < width)
	{
		createDownScaledTexture(height, width, tex);
		return;
	}
	// upscale
	if (tex_height < height || tex_width < width)
	{
		if (mode == "repeat")
		{
			createRepeatingTexture(height, width, tex);
			return;
		}
	//if (mode == "stretchtofit")
	//	createStretchToFitTexture(height, width, tex);
	}*/
}

void TextureMapper::horizontalDownscale(int height, int width)
{
	std::wstring tex = L"";
	tex.reserve(tex_height * (width + 1));
	if (tex_width > width)
	{
		for (int i = 0; i < tex_height; i++)
		{
			//tex += currentTextureMipMap.substr(i * tex_width + i, width);
			//tex += L"\n";
			tex.append(currentTextureMipMap.substr(i * tex_width + i, width));
			tex.push_back(L'\n');
		}
	}
	tex_width = width;
	TextureMapper::currentTextureMipMap = std::move(tex);
}

void TextureMapper::verticalDownscale(int height, int width)
{
	TextureMapper::currentTextureMipMap = TextureMapper::currentTextureMipMap.substr(0, width * height + height);
	tex_height = height;
}


void TextureMapper::createDownScaledTexture(int height, int width, std::wstring* tex)
{
	currentTextureMipMap = *tex;
	horizontalDownscale(height, width);
	verticalDownscale(height, width);
}

void TextureMapper::repeatingHorizontalUpscale(int height, int width)
{
	std::wstring line = L"";
	line.reserve(width);
	std::wstring new_tex = L"";
	new_tex.reserve(height * width);
	
	for (int i = 0; i < tex_height; i++)
	{
		/*
		new_tex.append(TextureMapper::currentTextureMipMap.substr((1 + tex_width) * i, tex_width));
		new_tex.append(TextureMapper::currentTextureMipMap.substr(0, width - tex_width));
		new_tex.push_back(L'\n');
		*/
		line = TextureMapper::currentTextureMipMap.substr((1 + tex_width) * i, tex_width);
		while (line.length() < width)
			line.append(line.substr(0, width - line.length()));
		new_tex.append(line);
		new_tex.push_back(L'\n');
	}
	TextureMapper::currentTextureMipMap = std::move(new_tex);
	tex_width = width;
}

void TextureMapper::repeatingVerticalUpscale(int height, int width)
{
	std::wstring new_tex = TextureMapper::currentTextureMipMap;
	int i = 0;
	while (tex_height < height)
	{
		new_tex += new_tex.substr((tex_width + 1) * i, tex_width);
		i = (i + 1) % tex_height;
		tex_height++;
	}
	TextureMapper::currentTextureMipMap = std::move(new_tex);
}


void TextureMapper::createRepeatingTexture(int height, int width, std::wstring* tex)
{
	TextureMapper::currentTextureMipMap = *tex;
	repeatingHorizontalUpscale(height, width);
	repeatingVerticalUpscale(height, width);
}

std::wstring TextureMapper::GetCharColumnAtPosition(int height) 
{
	if (tex_height < 1 || tex_width < 1)
	{
		return std::wstring(L"");
	}
	if (TextureMapper::stepper >= tex_width)
	{
		TextureMapper::stepper = 0;
	}
	std::wstring ret = L"";
	ret.reserve(height);

	for (int i = 0; i < tex_height; i++)
	{
		ret.push_back(TextureMapper::currentTextureMipMap[(i % tex_height) * tex_width + TextureMapper::stepper]);
	}
	int j = 0;
	while (ret.length() < height)
	{
		ret.push_back(TextureMapper::currentTextureMipMap[(j % tex_height) * tex_width + TextureMapper::stepper]);
		j++;
	}
	TextureMapper::stepper++;
	return ret;
}

std::wstring TextureMapper::ScaleToHeight(int height,  std::wstring column) {
	std::wstring r_column = L"";
	r_column.reserve(height);
	while (r_column.length() < height) {
		for (wchar_t c : column) {
			//temp += column[i];
			//r_column += column[i];
			r_column.push_back(c);
		}
	}
	return r_column.substr(0, height);
}
