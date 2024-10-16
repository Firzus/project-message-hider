#include "ImageComponent.h"

ImageComponent::ImageComponent(const std::wstring& imagePath, int posX, int posY, int width, int height)
    : posX(posX), posY(posY), width(width), height(height), image(nullptr)
{
    // Initialiser GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    SetImage(imagePath);
}

ImageComponent::~ImageComponent() {
    if (image) {
        delete image;
    }
}

void ImageComponent::SetImage(const std::wstring& imagePath) {
    if (image) {
        delete image;
    }

    // Vérifier si le fichier existe avant de charger l'image
    DWORD fileAttribs = GetFileAttributes(imagePath.c_str());
    if (fileAttribs == INVALID_FILE_ATTRIBUTES) {
        MessageBox(nullptr, L"Le fichier image spécifié est introuvable.", L"Erreur", MB_OK | MB_ICONERROR);
        return;
    }

    // Charger l'image avec GDI+
    image = Gdiplus::Image::FromFile(imagePath.c_str());

    // Vérifier si l'image a bien été chargée
    if (image->GetLastStatus() != Gdiplus::Ok) {
        MessageBox(nullptr, L"Impossible de charger l'image.", L"Erreur", MB_OK | MB_ICONERROR);
        delete image;
        image = nullptr;
    }
}

void ImageComponent::Draw(HDC hdc) {
    if (image) {
        Gdiplus::Graphics graphics(hdc);

        // Dessiner l'image avec redimensionnement
        graphics.DrawImage(image, posX, posY, width, height);
    }
}