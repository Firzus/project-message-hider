#include "FontManager.h"

FontManager::FontManager()
{
    hFontTitlePath = L"resources/fonts/Inter-ExtraBold.ttf";
    hFontSubtitlePath = L"resources/fonts/Inter-Medium.ttf";
    hFontParagraphPath = L"resources/fonts/Inter-SemiBold.ttf";
    hFontLeadPath = L"resources/fonts/Inter-Regular.ttf";
    hFontLargePath = L"resources/fonts/Inter-Bold.ttf";
    hFontSmallPath = L"resources/fonts/Inter-Light.ttf";
    hFontMutedPath = L"resources/fonts/Inter-Thin.ttf";
}
FontManager::~FontManager() {}

void FontManager::LoadFont(HWND hWnd)
{
    fontCountTitle = AddFontResourceEx(hFontTitlePath.c_str(), FR_PRIVATE, 0);

    if (fontCountTitle > 0) {
        hFontTitle = CreateFont(
            48, 0, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Inter"
        );
    }

    if (fontCountTitle == 0) {
        MessageBox(hWnd, L"Erreur : Impossible de charger une ou plusieurs polices.", L"Erreur", MB_OK | MB_ICONERROR);
    }
}