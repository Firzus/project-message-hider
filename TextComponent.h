#pragma once

#include <windows.h>
#include <string>

class TextComponent
{
	int posX, posY;
	COLORREF textColor;

public:
	TextComponent(int posX, int posY, COLORREF textColor);
	~TextComponent();
	void Draw(HDC hdc, HFONT font, const std::wstring& text, int frameWidth);
	void SetColor(HWND hWnd, COLORREF newColor);
	void DeleteText(HWND hWnd);
};

