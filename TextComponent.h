#pragma once

#include <windows.h>
#include <string>

class TextComponent
{
	COLORREF textColor;
	HFONT font;
	int posX, posY, frameWidth;
	std::wstring text;

public:
	TextComponent(HFONT initiaFont, const std::wstring& initialText, int posX, int posY, int initialFrameWidth, COLORREF textColor);
	~TextComponent();
	void Draw(HDC hdc);
	void SetColor(HWND hWnd, COLORREF newColor);
	void SetText(HWND hWnd, const std::wstring& newText);
	void DeleteText(HWND hWnd);
};

