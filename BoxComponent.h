#pragma once

#include <windows.h>

class BoxComponent
{
	int posX, posY, colorID;

public :
	BoxComponent(int posX, int posY, int colorID);
	~BoxComponent();
	void Draw(HDC hdc, int width, int height, COLORREF fillColor) const;
	void SetColor(HWND hWnd, int newColor);

	// Getter
	int GetColorID() const { return colorID; }
};