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

void ImageManager::paintImage(HDC hdc)
{
    // Charger l'image PNG avec WIC
    HBITMAP hBitmap = LoadPNGImage(filePath, hdc);

    if (hBitmap)
    {
        HDC hdcMem = CreateCompatibleDC(hdc);
        SelectObject(hdcMem, hBitmap);

        BITMAP bitmap;
        GetObject(hBitmap, sizeof(bitmap), &bitmap);


        // On limite la taille de l'image à afficher
        if (bitmap.bmWidth <= widthLimit && bitmap.bmHeight <= heightLimit)
        {
            actualImageWidth = bitmap.bmWidth;
            actualImageHeight = bitmap.bmHeight;

            // Fonction pour dessiner l'image
            BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
        }
        else
        {
            // Affiche une erreur car l'image a des dimensions au-dessus des dimensions fixées
            const wchar_t* errorMessage = L"Erreur: L'image est trop grande.";
            TextOut(hdc, 10, 10, errorMessage, lstrlenW(errorMessage));
        }

        DeleteDC(hdcMem);
        DeleteObject(hBitmap);
    }
    else
    {
        // Affiche une erreur car l'image n'a pas pu être chargée
        const wchar_t* errorMessage = L"Erreur: Impossible de charger l'image.";
        TextOut(hdc, 10, 10, errorMessage, lstrlenW(errorMessage));
    }
}
