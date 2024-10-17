#include "MessageManager.h"


bool MessageManager::HideMessage(const std::wstring& ImagePath, const std::wstring& Message, HDC hdc)
{
    // load the bmp file
    HBITMAP hBitmap = GetBitMapFromImage(ImagePath.c_str(), hdc);  //(HBITMAP)LoadImage(NULL, ImagePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!hBitmap) {
        OutputDebugStringA("Error while loading image\n");
        return false;
    }
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    std::vector<int> messageBits = ConvertStringToBits(Message);
    if (!SaveMessage(hBitmap, bmp, messageBits)) {
        OutputDebugStringA("Error while writing the message.\n");
        return false;
    }
    
    bool isPng = (ImagePath.find(L".png") != std::wstring::npos);
    std::wstring outputPath = ImagePath.substr(0, ImagePath.find('.')) + (isPng ? L"_encrypted.png" : L"_encrypted.jpg");
    if (!SaveBitmapAsImage(hBitmap, outputPath, hdc, isPng)) {
        OutputDebugStringA("Error while saving image.\n");
        return false;
    }
    DeleteObject(hBitmap);
    return true;
}

std::wstring MessageManager::GetMessage(const std::wstring& ImagePath, HDC hdc)
{
    HBITMAP hBitmap = GetBitMapFromImage(ImagePath.c_str(), hdc);
    BITMAP bmp;
    std::vector<int> messageBits;
    BYTE* pixels;
    int bitsPerPixel = 0;
    std::wstring embededMessage = L"";

    if (!hBitmap) {
        OutputDebugStringA("Error while loading image\n");
        return L"";
    }
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    if (bmp.bmBitsPixel != 24 && bmp.bmBitsPixel != 32) {
        OutputDebugStringA("Pixel size not valid use 24 or 32 pixel size\n");
        return L"";
    }
    pixels = (BYTE*)bmp.bmBits;
    bitsPerPixel = bmp.bmBitsPixel / 8;

    for (int y = 0; y < bmp.bmHeight; ++y) {
        for (int x = 0; x < bmp.bmWidth; ++x) {
            int pixelIndex = (y * bmp.bmWidthBytes) + (x * bitsPerPixel);
            int blue = pixels[pixelIndex]; //(BGR)
            messageBits.push_back(blue & 1);
            if (messageBits.size() >= 8 && messageBits.size() % 8 == 0) {
                char ch = GetCharFromByte(messageBits, messageBits.size() - 8);
                embededMessage += ch;
                if (ch == '\0')
                    return SaveMessageInFile(embededMessage, (ImagePath.substr(0, ImagePath.find('.')) + L"_message.txt"));
            }
        }
    }
}

std::vector<int> MessageManager::ConvertStringToBits(const std::wstring& Message)
{
    std::vector<int> messageBits;
    std::wstring mess = Message + L'\0';
    for (int i = 0; i < mess.size(); i++)
        for (int j = 0; j < 8; j++)
            messageBits.push_back((mess[i] >> j) & 1);
            // extracts the i-th bit from mess[i] and adds it to the messageBits list
    return messageBits;
}

bool MessageManager::SaveMessage(HBITMAP HBitmap, BITMAP& Bmp, const std::vector<int>& MessageBits)
{
    int bitIndex = 0;
    // 8 bits                   img data starts
    BYTE* pixels = (BYTE*)Bmp.bmBits;
    int bytesPerPixel = Bmp.bmBitsPixel / 8;

    // check pixels size
    if (Bmp.bmBitsPixel != 24 && Bmp.bmBitsPixel != 32) {
        OutputDebugStringA("Pixel size not valid use 24 or 32 pixel size");
        return false;
    }

    for (int y = 0; y < Bmp.bmHeight; y++) {
        for (int x = 0; x < Bmp.bmWidth; x++) {
            if (bitIndex >= MessageBits.size()) 
                return true;
            int pixelIndex = (y * Bmp.bmWidthBytes) + (x * bytesPerPixel); // (height * nb per lines) + (width * 3)
            int blue = pixels[pixelIndex]; // get the blue channel as pixel pos in img (B, G, R)
            int lsb = blue & 1;
            if (lsb != MessageBits[bitIndex]) {
                blue += (rand() % 2 == 0) ? 1 : -1; // increase or decrease pixel value
                blue = (blue < 0) ? 0 : (blue > 255) ? 255 : blue;
                pixels[pixelIndex] = blue;
            }
            bitIndex++;
        }
    }
    return true;
}

HBITMAP MessageManager::GetBitMapFromImage(LPCWSTR filePath, HDC hdc)
{
    // Créer un objet WIC
    IWICImagingFactory* pFactory = nullptr;
    CoInitialize(nullptr);
    CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFactory));

    // Charger l'image PNG
    IWICBitmapDecoder* pDecoder = nullptr;
    HRESULT hr = pFactory->CreateDecoderFromFilename(filePath, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
    if (FAILED(hr)) {
        OutputDebugStringA("Error while loading image\n");
        return nullptr;
    }

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

    pConverter->Release();
    pFrame->Release();
    pDecoder->Release();
    pFactory->Release();

    return hTempBitmap;
}

bool MessageManager::SaveBitmapAsImage(HBITMAP hBitmap, const std::wstring& outputPath, HDC hdc, bool isPng)
{
    IWICImagingFactory* pFactory = nullptr;
    CoInitialize(nullptr);
    CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFactory));

    //IWICBitmap* pWicBitmap = nullptr;
    IWICBitmapEncoder* pEncoder = nullptr;
    IWICBitmapFrameEncode* pFrameEncode = nullptr;
    IWICStream* pStream = nullptr;

    HRESULT hr = pFactory->CreateStream(&pStream);
    if (FAILED(hr))
        return false;
    hr = pStream->InitializeFromFilename(outputPath.c_str(), GENERIC_WRITE);
    if (FAILED(hr))
        return false;
    
    GUID containerFormat = isPng ? GUID_ContainerFormatPng : GUID_ContainerFormatJpeg;
    hr = pFactory->CreateEncoder(containerFormat, nullptr, &pEncoder);
    
    
    if (FAILED(hr))
        return false;
    hr = pEncoder->Initialize(pStream, WICBitmapEncoderNoCache);
    if (FAILED(hr))
        return false;
    IPropertyBag2 * pPropertyBag = nullptr;
    hr = pEncoder->CreateNewFrame(&pFrameEncode, &pPropertyBag);
    if (FAILED(hr))
        return false;

    if (!isPng) {
        PROPBAG2 option = { 0 };
        option.pstrName = const_cast<LPOLESTR>(L"ImageQuality");
        VARIANT varValue;
        VariantInit(&varValue);
        varValue.vt = VT_R4;
        varValue.fltVal = 0.95f;

        hr = pPropertyBag->Write(1, &option, &varValue);
        VariantClear(&varValue);
        if (FAILED(hr)) {
            pPropertyBag->Release();
            return false;
        }
    }
    hr = pFrameEncode->Initialize(pPropertyBag);
    if (pPropertyBag)
        pPropertyBag->Release();
    if (FAILED(hr))
         return false;
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    UINT width = bmp.bmWidth;
    UINT height = bmp.bmHeight;
    int bitsPerPixel = bmp.bmBitsPixel;
    hr = pFrameEncode->SetSize(width, height);
    if (FAILED(hr))
        return false;
    WICPixelFormatGUID pixelFormat = (bitsPerPixel == 32) ? GUID_WICPixelFormat32bppBGRA : (bitsPerPixel == 24) ? GUID_WICPixelFormat24bppBGR : GUID_WICPixelFormatDontCare;
    if (pixelFormat == GUID_WICPixelFormatDontCare)
        return false;
    hr = pFrameEncode->SetPixelFormat(&pixelFormat);
    if (FAILED(hr))
        return false;
    hr = pFrameEncode->WritePixels(height, bmp.bmWidthBytes, bmp.bmWidthBytes * height, (BYTE*)bmp.bmBits);
    if (FAILED(hr))
        return false;
    hr = pFrameEncode->Commit();
    if (FAILED(hr))
        return false;
    hr = pEncoder->Commit();
    if (FAILED(hr))
        return false;
    pStream->Release();
    pFrameEncode->Release();
    pEncoder->Release();
    pFactory->Release();
    return true;
}

char MessageManager::GetCharFromByte(const std::vector<int>& BitsMap, int Index)
{
    char ch = '\0';
    for (int i = 0; i < 8 && Index + i < BitsMap.size(); i++) {
        // != add the shifted bit to ch bit by bit
        ch |= (BitsMap[Index + i] << i);  // get the BitsMap[Index + i] bit and shif it of i pos by left
    }
    return ch;
}

std::wstring MessageManager::SaveMessageInFile(std::wstring embededMessage, std::wstring fileName)
{
    std::wofstream file(fileName, std::ios::binary);
    
    if (file.is_open()) {
        file.write(embededMessage.c_str(), embededMessage.size());
        file.close();
    }
    return fileName;
}

