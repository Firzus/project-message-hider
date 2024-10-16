#pragma once

#include <windows.h>
#include <string>

class TextComponent
{
public:

	TextComponent(HDC hdc, const std::wstring& text, int posX, int posY, HFONT font, COLORREF textColor, int frameWidth);
	~TextComponent();	
};

