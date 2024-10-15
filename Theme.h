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
	COLORREF GetColor(int index);
	std::string GetColorHex(int index);
};

