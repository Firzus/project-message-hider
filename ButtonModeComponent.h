#pragma once

#include <windows.h>

class ButtonModeComponent {
    int id, height, width, posY, posX, inRectColorID, outRectColorID;
	bool isOn;

public:
    ButtonModeComponent(int x, int y, int width, int height, int id, int outRectColorID, int inRectColorID);
    void Draw(HDC hdc, COLORREF outRectColor, COLORREF inRectColor);
    bool HitTest(int mouseX, int mouseY) const;
    void OnClick();
    void MoveInnerRectangle(bool isOn);

    // Getter
	int GetInRectColorID() const { return inRectColorID; }
	int GetOutRectColorID() const { return outRectColorID; }
	bool GetIsOn() const { return isOn; }
};