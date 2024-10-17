#pragma once

#include <windows.h>

class BoxComponent
{
	int posX, posY;
	COLORREF fillColor; // Couleur actuelle de la box

public :
	BoxComponent(int posX, int posY, COLORREF fillColor);
	~BoxComponent();
	void Draw(HDC hdc, int width, int height);
	void SetColor(HWND hWnd, COLORREF newColor);
	void DeleteBox(HWND hWnd);
};

