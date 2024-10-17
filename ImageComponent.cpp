#include "ImageComponent.h"

ImageComponent::ImageComponent(const std::wstring& imagePath, int posX, int posY, int width, int height)
    : posX(posX), posY(posY), width(width), height(height), image(nullptr)
{
    // Initialise GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    SetImage(imagePath);
}

ImageComponent::ImageComponent()
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
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

    // Vérifie si le fichier existe avant de charger l'image
    DWORD fileAttribs = GetFileAttributes(imagePath.c_str());
    if (fileAttribs == INVALID_FILE_ATTRIBUTES) {
        MessageBox(nullptr, L"Le fichier image spécifié est introuvable.", L"Erreur", MB_OK | MB_ICONERROR);
        return;
    }

    // Charge l'image
    image = Gdiplus::Image::FromFile(imagePath.c_str());

    // Vérifie si l'image a bien été chargée
    if (image->GetLastStatus() != Gdiplus::Ok) {
        MessageBox(nullptr, L"Impossible de charger l'image.", L"Erreur", MB_OK | MB_ICONERROR);
        delete image;
        image = nullptr;
    }
}


HBITMAP ImageComponent::LoadPNGImage(LPCWSTR filePath)
{
    // Crée un objet Bitmap GDI+ à partir du fichier PNG
    Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(filePath, FALSE);

    // Vérifie si le chargement fonctionne
    if (bitmap == nullptr || bitmap->GetLastStatus() != Gdiplus::Ok)
    {
        return nullptr;
    }

    HBITMAP hBitmap;
    bitmap->GetHBITMAP(Gdiplus::Color(0, 0, 0), &hBitmap);

    delete bitmap;

    return hBitmap;
}

bool ImageComponent::IsValidFile(LPCWSTR filePath)
{
    // Récupère l'extension du fichier déposé
    std::wstring path(filePath);
    size_t extPos = path.find_last_of(L".");

    if (extPos == std::wstring::npos)
        return false;  // Pas d'extension trouvée

    std::wstring extension = path.substr(extPos + 1);

    // Vérifie si l'extension du fichier 
    // est bien une extension valide (png, jpg, jpeg)
    return (extension == L"png" || extension == L"PNG"
        || extension == L"jpg" || extension == L"JPG" 
        || extension == L"jpeg" || extension == L"JPEG");
}

void ImageComponent::PaintImage(HDC hdc, HWND hwnd, LPCWSTR filePath)
{
    // Charge l'image PNG avec GDI+
    hBitmap = LoadPNGImage(filePath);

    if (hBitmap)
    {
        HDC hdcMem = CreateCompatibleDC(hdc);
        SelectObject(hdcMem, hBitmap);

        BITMAP bitmap;
        GetObject(hBitmap, sizeof(bitmap), &bitmap);

        // Limite les dimensions
        if (bitmap.bmWidth > dimensionLimit && bitmap.bmHeight > dimensionLimit)
        {
            // Affiche une erreur si les dimensions de l'image sont trop grandes

            MessageBox(hwnd, L"Erreur: L'image est trop grande.", L"Erreur de dimensions", MB_OK | MB_ICONERROR);
        }
        else
        {
            // Calcul de la hauteur de la barre de titre
            titleBarHeight = GetSystemMetrics(SM_CYCAPTION);

            // Calcul de la hauteur de la zone de travail
            SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
            workAreaHeight = workArea.bottom - workArea.top;

            // Nouveau calcul des dimensions de la preview pour prendre en compte
            // la taille de la barre de titre et la barre des tâches
            squareSize = workAreaHeight - (titleBarHeight + uploadedImagePosY + spaceBetweenObjects);

            // Nouveau calcul de la position X pour que la preview soit toujours alignée
            uploadedImagePosX = 1920 - spaceBetweenObjects - squareSize;

            // Calcul des dimensions à afficher (redimensionner pour entrer dans un carré)
            int srcWidth = (bitmap.bmWidth > squareSize) ? squareSize : bitmap.bmWidth;
            int srcHeight = (bitmap.bmHeight > squareSize) ? squareSize : bitmap.bmHeight;

            // Affiche l'image dans la fenêtre avec les dimensions spécifiées
            StretchBlt(hdc, uploadedImagePosX, uploadedImagePosY, squareSize, squareSize, hdcMem, 0, 0, srcWidth, srcHeight, SRCCOPY);
        }

        DeleteDC(hdcMem);
        DeleteObject(hBitmap);
    }
    else
    {
        // Affiche une erreur si l'image n'a pas pu être chargée
        MessageBox(hwnd, L"Erreur: Impossible de charger l'image.", L"Erreur de chargement", MB_OK | MB_ICONERROR);
    }

}



void ImageComponent::Draw(HDC hdc) {
    if (image) {
        Gdiplus::Graphics graphics(hdc);

        // Dessiner l'image avec redimensionnement
        graphics.DrawImage(image, posX, posY, width, height);
    }
}