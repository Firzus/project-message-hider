#pragma once
#include <windows.h>

class Button
{
    HWND hWndParent;    // Handle to parent window
    HWND hButton;       // Handle to the button control
    int id;             // Button ID
    RECT rect;          // Position and size
    bool isPrimary;     // Boolean to switch between styles
    COLORREF bgColor;   // Background color
    COLORREF textColor; // Text color

public:
    Button(HWND parent, int x, int y, int width, int height, int buttonId, bool primaryStyle);
    ~Button();

    void UpdateColors();
    void Draw(HDC hdc);
    void OnClick() const;
    void SetStyle(bool primaryStyle);
    HWND GetHandle() const;
};
