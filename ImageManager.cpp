#include "ImageManager.h"

HBITMAP ImageManager::LoadPNGImage(LPCWSTR filePath, HDC hdc)
{
    // Créer un objet WIC
    IWICImagingFactory* pFactory = nullptr;
    CoInitialize(nullptr);
    CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFactory));

    // Charger l'image PNG
    IWICBitmapDecoder* pDecoder = nullptr;
    pFactory->CreateDecoderFromFilename(filePath, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);

    // Obtenir le frame de l'image
    IWICBitmapFrameDecode* pFrame = nullptr;
    pDecoder->GetFrame(0, &pFrame);

    // Convertir l'image en 32 bits (ARGB)
    IWICFormatConverter* pConverter = nullptr;
    pFactory->CreateFormatConverter(&pConverter);
    pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);

    // Créer un bitmap compatible GDI à partir des données WIC
    HBITMAP hBitmap = nullptr;
    UINT width, height;
    pFrame->GetSize(&width, &height);

    BITMAPINFO bitmapInfo;
    memset(&bitmapInfo, 0, sizeof(bitmapInfo));
    bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo.bmiHeader.biWidth = width;
    bitmapInfo.bmiHeader.biHeight = -(LONG)height;  // Négatif pour un top-down bitmap
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 32;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;

    void* pvImageBits = nullptr;
    HBITMAP hTempBitmap = CreateDIBSection(hdc, &bitmapInfo, DIB_RGB_COLORS, &pvImageBits, nullptr, 0);

    pConverter->CopyPixels(nullptr, width * 4, width * height * 4, (BYTE*)pvImageBits);

    // Nettoyage
    pConverter->Release();
    pFrame->Release();
    pDecoder->Release();
    pFactory->Release();

    return hTempBitmap;
}




void ImageManager::paintImage(HDC hdc, HWND hwnd, LPCWSTR filePath)
{
    // Charger l'image PNG avec WIC
    hBitmap = LoadPNGImage(filePath, hdc);

    if (hBitmap)
    {
        HDC hdcMem = CreateCompatibleDC(hdc);
        SelectObject(hdcMem, hBitmap);

        BITMAP bitmap;
        GetObject(hBitmap, sizeof(bitmap), &bitmap);


        // On limite les images qu'on peut importer en fonction de leurs dimensions
        if (bitmap.bmWidth > dimensionLimit && bitmap.bmHeight > dimensionLimit)
        {
            // Affiche une erreur car l'image a des dimensions au-dessus des dimensions fixées
            MessageBox(hwnd, L"Erreur: L'image est trop grande.", L"Erreur de dimensions", MB_OK | MB_ICONERROR);
        }
        else
        {
            // Calcul des dimensions à afficher (réduit ou agrandit l'image pour qu'elle tienne dans le carré)
            int srcWidth = (bitmap.bmWidth > squareSize) ? squareSize : bitmap.bmWidth;
            int srcHeight = (bitmap.bmHeight > squareSize) ? squareSize : bitmap.bmHeight;

            // Affiche l'image dans le carré
            StretchBlt(hdc, posX, posY, squareSize, squareSize, hdcMem, 0, 0, srcWidth, srcHeight, SRCCOPY);
        }

        DeleteDC(hdcMem);
    }
    else
    {
        // Affiche une erreur car l'image n'a pas pu être chargée
        MessageBox(hwnd, L"Erreur: Impossible de charger l'image.", L"Erreur inconnue", MB_OK | MB_ICONERROR);
    }
}

bool ImageManager::IsPNGFile(LPCWSTR filePath)
{
    std::wstring path(filePath);
    // Récupère l'extension du fichier déposé
    size_t extPos = path.find_last_of(L".");
    if (extPos == std::wstring::npos)
        return false;  // Pas d'extension trouvée

    // Vérifie si l'extension du fichier est bien "png"
    std::wstring extension = path.substr(extPos + 1);
    if (extension == L"png" || extension == L"PNG")
    {
        return true;
    }
    return false;
}

