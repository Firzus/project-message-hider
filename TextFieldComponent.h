#pragma once
#include <string>
#include <windows.h>

class TextFieldComponent
{
public:
    TextFieldComponent(HWND parentHandle, HINSTANCE hInstance, int x, int y, int width, int height);
    ~TextFieldComponent();
    std::wstring GetText();
    void DrawUnderline();
    std::wstring UpdateCharCount();
    HWND GetHandle() const {
        return hTextField;
    }
private:
    HWND hTextField;
    HWND hParent;
    HINSTANCE hInst;
    int _x;
    int _y;
    int _width;
    int _height;
    const int maxChars = 1000;
};

