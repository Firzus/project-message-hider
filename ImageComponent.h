#pragma once

#include <string>
#include <windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

class ImageComponent {
public:
    ImageComponent(const std::wstring& imagePath, int posX, int posY, int width, int height);
    ~ImageComponent();

    void Draw(HDC hdc);
    void SetImage(const std::wstring& imagePath);

private:
    Gdiplus::Image* image;
    int posX, posY;
    int width, height;
};
