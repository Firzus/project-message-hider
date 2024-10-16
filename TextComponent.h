#pragma once

#include <windows.h>
#include <string>

class TextComponent
{
	int posX, posY; // Position du texte
	const std::wstring& text;
	COLORREF textColor; // Couleur du texte
	int frameWidth; // Largeur du cadre

public:
	TextComponent(HDC hdc, const std::wstring& text, int posX, int posY, int frameWidth, HFONT font, COLORREF textColor);
	~TextComponent();
	void SetColor(HWND hWnd, COLORREF newColor);
};

