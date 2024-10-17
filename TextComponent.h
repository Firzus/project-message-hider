#pragma once

#include <windows.h>
#include <string>

class TextComponent
{
	HFONT font;
	int posX, posY, frameWidth, colorID;
	std::wstring text;

public:
	TextComponent(HFONT initiaFont, const std::wstring& initialText, int posX, int posY, int initialFrameWidth, int colorID);
	~TextComponent();
	void Draw(HDC hdc, COLORREF textColor);
	void SetColor(HWND hWnd, int newColorID);
	void SetText(HWND hWnd, const std::wstring& newText);
	void DeleteText(HWND hWnd);

	// Getter
	int GetColorID() const { return colorID; }
};

