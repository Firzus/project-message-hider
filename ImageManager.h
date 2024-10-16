#pragma once
#include "framework.h"

class ImageManager
{
public:
	void paintImage(HDC hdc, HWND hwnd, LPCWSTR filePath);
	bool IsPNGFile(LPCWSTR filePath);
	UINT actualImageDimensions = 400;
	HBITMAP hBitmap;

private:
	HBITMAP LoadPNGImage(LPCWSTR filePath, HDC hdc);
	int dimensionLimit = 980;
};