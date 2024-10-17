#include "ImageComponent.h"

ImageComponent::ImageComponent(const std::wstring& initialImagePath, int posX, int posY, int width, int height) : posX(posX), posY(posY), width(width), height(height), imagePath(initialImagePath)
{
    CalculatePreviewDimensions();

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

    // Dessiner l'image avec redimensionnement
    graphics.DrawImage(image, posX, posY, width, height);
}

void ImageComponent::CalculatePreviewDimensions()
{
    // Calcul de la hauteur de la barre de titre
    titleBarHeight = GetSystemMetrics(SM_CYCAPTION);

    // Calcul de la hauteur de la zone de travail
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
    workAreaHeight = workArea.bottom - workArea.top;

    // Nouveau calcul des dimensions de la preview pour prendre en compte
    // la taille de la barre de titre et la barre des t�ches
    squareSize = workAreaHeight - (titleBarHeight + uploadedImagePosY + spaceBetweenObjects);

    // Nouveau calcul de la position X pour que la preview soit toujours align�e
    uploadedImagePosX = 1920 - spaceBetweenObjects - squareSize;
}