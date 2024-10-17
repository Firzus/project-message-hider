#pragma once

#include <windows.h>

class ButtonComponent {
    int id, height, width, posY, posX;
    const wchar_t* text;
    COLORREF colorLight, colorDark;
	bool isPrimary; // Style du bouton

public:
    ButtonComponent(int x, int y, int width, int height, const wchar_t* text, int id, bool isPrimary);
    void Draw(HDC hdc);
    bool HitTest(int mouseX, int mouseY) const;
    void OnClick();
    void DeleteButton(HWND hWnd);

    // Get
    int GetId() const { return id; }
};