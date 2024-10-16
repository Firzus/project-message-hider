#pragma once
#include "framework.h"

class ImageManager
{
public:
	void paintImage(HDC hdc, HWND hwnd, LPCWSTR filePath);
	bool IsPNGFile(LPCWSTR filePath);
	HBITMAP hBitmap;

private:
	HBITMAP LoadPNGImage(LPCWSTR filePath, HDC hdc);
	int dimensionLimit = 980;

	// Position et taillede l'image dans la fenêtre
	int posX = 500;
	int posY = 200;
	int squareSize = 300;
};