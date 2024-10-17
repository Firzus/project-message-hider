#pragma once

#include <string>
#include <windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

class ImageComponent {
	std::wstring imagePath;

    Gdiplus::Image* image;
    int posX, posY;
    int width, height;
    bool isAnImageLoaded = false;

public:
    ImageComponent(const std::wstring& imagePath, int posX, int posY, int width, int height);
    ~ImageComponent();

    void Draw(HDC hdc);
    bool IsValidFile(LPCWSTR filePath);
    HBITMAP hBitmap;

	// Getters
	std::wstring GetImagePath() const { return imagePath; }
    bool GetIsAnImageLoaded() const { return isAnImageLoaded; }
};
