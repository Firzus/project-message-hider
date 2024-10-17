#pragma once

#include <windows.h>

class BoxComponent
{
	int posX, posY;
	COLORREF fillColor;

public :
	BoxComponent(int posX, int posY, COLORREF initialFillColor);
	~BoxComponent();
	void Draw(HDC hdc, int width, int height);
	void SetColor(HWND hWnd, COLORREF newColor);
};