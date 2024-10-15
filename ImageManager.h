#pragma once
#include "framework.h"

class ImageManager
{
public:
	void paintImage(HDC hdc);
	UINT actualImageDimensions = 400;

private:
	HBITMAP LoadPNGImage(LPCWSTR filePath, HDC hdc);
	int dimensionLimit = 1200;
	LPCWSTR filePath = L"C:/Users/theob/OneDrive/Bureau/GTech 4 - Projet 2/Images/Paysage.png";
};