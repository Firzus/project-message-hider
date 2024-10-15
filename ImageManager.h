#pragma once
#include "framework.h"

class ImageManager
{
public:
	void paintImage(HDC hdc);
	UINT actualImageWidth = 400;
	UINT actualImageHeight = 400;

private:
	HBITMAP LoadPNGImage(LPCWSTR filePath, HDC hdc);
	int widthLimit = 1200;
	int heightLimit = 1200;
	LPCWSTR filePath = L"C:/Users/theob/OneDrive/Bureau/GTech 4 - Projet 2/Images/Ballon.png";
};

