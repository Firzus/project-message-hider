#pragma once
#include "framework.h"

class ImageManager
{
public:
	void paintImage(HDC hdc);
	UINT actualImageWidth;
	UINT actualImageHeight;

private:
	HBITMAP LoadPNGImage(LPCWSTR filePath, HDC hdc);
};

