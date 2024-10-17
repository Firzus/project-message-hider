#pragma once

#include <string>
#include <windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

class ImageComponent {
	const std::wstring& imagePath;

    Gdiplus::Image* image;
    int posX, posY;
    int width, height;
    bool isAnImageLoaded = false;

    // Position et taille de l'image dans la fen�tre
    int dimensionLimit = 980;
    int uploadedImagePosX = 1381;
    int uploadedImagePosY = 541;
    int squareSize = 463;

    // Informations sur l'�cran pour positionner l'image correctement
    RECT workArea;
    int workAreaHeight;
    int titleBarHeight;
    int spaceBetweenObjects = 48;

public:
    ImageComponent(const std::wstring& imagePath, int posX, int posY, int width, int height);
    ~ImageComponent();

    void Draw(HDC hdc);
    void CalculatePreviewDimensions();
    bool IsValidFile(LPCWSTR filePath);
    HBITMAP hBitmap;

	// Getters
	const std::wstring& GetImagePath() const { return imagePath; }
    bool GetIsAnImageLoaded() const { return isAnImageLoaded; }
};
