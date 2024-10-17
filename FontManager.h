#pragma once
#include <string>
#include <Windows.h>

class FontManager
{
	std::wstring hFontTitlePath;
	std::wstring hFontSubtitlePath;
	std::wstring hFontParagraphPath;
	std::wstring hFontLeadPath;
	std::wstring hFontLargePath;
	std::wstring hFontSmallPath;
	std::wstring hFontMutedPath;

	int fontCountTitle = 0;
	int fontCountSubtitle = 0;
	int fontCountParagraph = 0;
	int fontCountLead = 0;
	int fontCountLarge = 0;
	int fontCountSmall = 0;
	int fontCountMuted = 0;

	HFONT hFontTitle;
	HFONT hFontSubtitle;
	HFONT hFontParagraph;
	HFONT hFontLead;
	HFONT hFontLarge;
	HFONT hFontSmall;
	HFONT hFontMuted;

public:
	FontManager();
	~FontManager();

	void LoadFont(HWND hWnd);

	HFONT GetFontTitle() const { return hFontTitle; }
	HFONT GetFontSubtitle() const { return hFontSubtitle; }
	HFONT GetFontParagraph() const { return hFontParagraph; }
	HFONT GetFontLead() const { return hFontLead; }
	HFONT GetFontLarge() const { return hFontLarge; }
	HFONT GetFontSmall() const { return hFontSmall; }
	HFONT GetFontMuted() const { return hFontMuted; }
};

