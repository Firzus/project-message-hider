#pragma once

#include <string>
#include <windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

class ImageComponent {
    HBITMAP LoadPNGImage(LPCWSTR filePath);

	const std::wstring& imagePath;

    Gdiplus::Image* image;
    int posX, posY;
    int width, height;

    // Position et taille de l'image dans la fenêtre
    int dimensionLimit = 980;
    int uploadedImagePosX = 1381;
    int uploadedImagePosY = 541;
    int squareSize = 463;

    // Informations sur l'écran pour positionner l'image correctement
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
    void PaintImage(HDC hdc, HWND hwnd, LPCWSTR filePath);
    HBITMAP hBitmap;

	// Getters
	const std::wstring& GetImagePath() const { return imagePath; }
};
