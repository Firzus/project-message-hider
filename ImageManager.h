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
};

