#pragma once

#include <windows.h>

class ButtonComponent {
    int id, height, width, posY, posX;
    COLORREF colorLight, colorDark;
	bool isPrimary;

public:
    ButtonComponent(int x, int y, int width, int height, int id, bool isPrimary);
    void Draw(HDC hdc, const wchar_t* text);
    bool HitTest(int mouseX, int mouseY) const;
    void OnClick();
    void SetStyle(HWND hWnd, bool isPrimary);

    // Get
    int GetId() const { return id; }
	bool GetIsPrimary() const { return isPrimary; }
};