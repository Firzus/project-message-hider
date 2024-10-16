#pragma once
#include "framework.h"

class ImageManager
{
public:
	void paintImage(HDC hdc, HWND hwnd);
	UINT actualImageDimensions = 400;

private:
	HBITMAP LoadPNGImage(LPCWSTR filePath, HDC hdc);
	int dimensionLimit = 980;
	LPCWSTR filePath = L"C:/Users/theob/OneDrive/Bureau/GTech 4 - Projet 2/Images/hd2.png";
};