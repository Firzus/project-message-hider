#pragma once

#include <string>
#include <windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

class ImageComponent {
public:
    ImageComponent(const std::wstring& imagePath, int posX, int posY, int width, int height);
    // Deuxième constructeur exclusivement pour les images qu'on upload
    ImageComponent();
    ~ImageComponent();

    void Draw(HDC hdc);
    void SetImage(const std::wstring& imagePath);

    bool IsPNGFile(LPCWSTR filePath);
    void PaintImage(HDC hdc, HWND hwnd, LPCWSTR filePath);
    HBITMAP hBitmap;

private:
    HBITMAP LoadPNGImage(LPCWSTR filePath);

    Gdiplus::Image* image;
    int posX, posY;
    int width, height;

    // Position et taille de l'image dans la fenêtre
    int dimensionLimit = 980;
    int uploadedImagePosX = 1400;
    int uploadedImagePosY = 550;
    int squareSize = 400;
};
