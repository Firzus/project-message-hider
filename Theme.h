#pragma once
#include <windows.h>
#include <map>
#include <string>

class Theme
{
	std::map<int, COLORREF> colorPalette;

public:
	Theme();
	~Theme();

	void AddColor(int index, COLORREF color);
	void InvertColors();
	COLORREF GetColor(int index);
};

