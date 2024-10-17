#include "FontManager.h"

FontManager::FontManager() : hFontTitle(NULL), hFontSubtitle(NULL), hFontParagraph(NULL), hFontLarge(NULL), hFontSmall(NULL), hFontMuted(NULL)
{
    hFontTitlePath = L"resources/fonts/Inter-ExtraBold.ttf";
    hFontSubtitlePath = L"resources/fonts/Inter-SemiBold.ttf";
    hFontParagraphPath = L"resources/fonts/Inter-Regular.ttf";
    hFontLargePath = L"resources/fonts/Inter-SemiBold.ttf";
    hFontSmallPath = L"resources/fonts/Inter-Medium.ttf";
    hFontMutedPath = L"resources/fonts/Inter-Regular.ttf";
}
FontManager::~FontManager() {}

void FontManager::LoadFont(HWND hWnd)
{
    fontCountTitle = AddFontResourceEx(hFontTitlePath.c_str(), FR_PRIVATE, 0);
	fontCountSubtitle = AddFontResourceEx(hFontSubtitlePath.c_str(), FR_PRIVATE, 0);
	fontCountParagraph = AddFontResourceEx(hFontParagraphPath.c_str(), FR_PRIVATE, 0);
	fontCountLarge = AddFontResourceEx(hFontLargePath.c_str(), FR_PRIVATE, 0);
	fontCountSmall = AddFontResourceEx(hFontSmallPath.c_str(), FR_PRIVATE, 0);
	fontCountMuted = AddFontResourceEx(hFontMutedPath.c_str(), FR_PRIVATE, 0);

    if (fontCountTitle > 0) {
        hFontTitle = CreateFont(
            48, 0, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Inter"
        );
    }

	if (fontCountSubtitle > 0) {
		hFontSubtitle = CreateFont(
			24, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Inter"
		);
	}

	if (fontCountParagraph > 0) {
		hFontParagraph = CreateFont(
			16, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Inter"
		);
	}

	if (fontCountLarge > 0) {
		hFontLarge = CreateFont(
			18, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Inter"
		);
	}

	if (fontCountSmall > 0) {
		hFontSmall = CreateFont(
			14, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Inter"
		);
	}

	if (fontCountMuted > 0) {
		hFontMuted = CreateFont(
			14, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Inter"
		);
	}

    if (fontCountTitle == 0 || fontCountSubtitle == 0 || fontCountParagraph == 0 || fontCountLarge == 0 || fontCountSmall == 0 || fontCountMuted == 0) {
        MessageBox(hWnd, L"Erreur : Impossible de charger une ou plusieurs polices.", L"Erreur", MB_OK | MB_ICONERROR);
    }
}