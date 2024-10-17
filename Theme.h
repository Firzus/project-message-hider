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

	COLORREF GetColor(int index);
	void ToggleMode();
};

