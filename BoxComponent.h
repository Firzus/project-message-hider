#pragma once

#include <windows.h>

class BoxComponent
{
	int posX, posY; // Position de la box
	int width, height; // Dimensions de la box
	COLORREF fillColor; // Couleur actuelle de la box

public :
	BoxComponent(HDC hdc, int posX, int posY, int width, int height, COLORREF fillColor);
	~BoxComponent();
	void SetColor(HWND hWnd, COLORREF newColor);
};

