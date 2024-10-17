#include "ImageComponent.h"

ImageComponent::ImageComponent(const std::wstring& initialImagePath, int posX, int posY, int width, int height) : posX(posX), posY(posY), width(width), height(height), imagePath(initialImagePath)
{
    if (image) {
        delete image;
    }

    // V�rifie si le fichier existe avant de charger l'image
    DWORD fileAttribs = GetFileAttributes(imagePath.c_str());
    if (fileAttribs == INVALID_FILE_ATTRIBUTES) {
        MessageBox(nullptr, L"Le fichier image sp�cifi� est introuvable.", L"Erreur", MB_OK | MB_ICONERROR);
        return;
    }

    // Charge l'image
    image = Gdiplus::Image::FromFile(imagePath.c_str());

    // V�rifie si l'image a bien �t� charg�e
    if (image->GetLastStatus() != Gdiplus::Ok) {
        MessageBox(nullptr, L"Impossible de charger l'image.", L"Erreur", MB_OK | MB_ICONERROR);
        delete image;
        image = nullptr;
    }
}

ImageComponent::~ImageComponent() {
    if (image) {
        delete image;
    }
}

bool ImageComponent::IsValidFile(LPCWSTR filePath)
{
    // R�cup�re l'extension du fichier d�pos�
    std::wstring path(filePath);
    size_t extPos = path.find_last_of(L".");

    if (extPos == std::wstring::npos)
        return false;  // Pas d'extension trouv�e

    std::wstring extension = path.substr(extPos + 1);

    // V�rifie si l'extension du fichier 
    // est bien une extension valide (png, jpg, jpeg)
    return (extension == L"png" || extension == L"PNG"
        || extension == L"jpg" || extension == L"JPG" 
        || extension == L"jpeg" || extension == L"JPEG");
}

void ImageComponent::Draw(HDC hdc) {
    Gdiplus::Graphics graphics(hdc);

    // Dimensions originales de l'image
    Gdiplus::REAL imgWidth = image->GetWidth();
    Gdiplus::REAL imgHeight = image->GetHeight();

    // Calcul de la taille pour un ratio 1:1
    Gdiplus::REAL squareSize = (imgWidth < imgHeight) ? imgWidth : imgHeight;

    // Calcul pour centrer l'image
    Gdiplus::REAL cropX = (imgWidth > imgHeight) ? (imgWidth - squareSize) / 2 : 0;
    Gdiplus::REAL cropY = (imgHeight > imgWidth) ? (imgHeight - squareSize) / 2 : 0;

    // Rect contenant la position et dimension de l'image rogn�e
    Gdiplus::RectF srcRect(cropX, cropY, squareSize, squareSize);

    // Rect contenant la position et dimension de l'image finale dans la fen�tre
    Gdiplus::RectF destRect(static_cast<Gdiplus::REAL>(posX), static_cast<Gdiplus::REAL>(posY),
        static_cast<Gdiplus::REAL>(width), static_cast<Gdiplus::REAL>(height));

    // Dessiner l'image rogn�e dans le rectangle de destination avec redimensionnement
    graphics.DrawImage(image, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, Gdiplus::UnitPixel);
}