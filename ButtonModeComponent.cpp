#include "ButtonModeComponent.h"

ButtonModeComponent::ButtonModeComponent(int x, int y, int width, int height, int id, int outRectColorID, int inRectColorID)
	: posX(x), posY(y), width(width), height(height), id(id), outRectColorID(outRectColorID), inRectColorID(inRectColorID), isOn(true) {}

void ButtonModeComponent::Draw(HDC hdc, COLORREF outRectColor, COLORREF inRectColor) {
    // Dessiner le bouton
    HBRUSH hBrush = CreateSolidBrush(outRectColor);
    RECT rect = { posX, posY, posX + width, posY + height };
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);

    // Utiliser les offsets pour dessiner le rectangle intérieur
    RECT innerRect = {
        rect.left + (isOn ? 54 : 8),  // x offset
        rect.top + 8,   // y offset
        rect.left + (isOn ? 54 : 8) + 34, // largeur du rectangle
        rect.top + 8 + 20   // hauteur du rectangle
    };

    HBRUSH hInnerBrush = CreateSolidBrush(inRectColor);
    FillRect(hdc, &innerRect, hInnerBrush);
    DeleteObject(hInnerBrush);
}

bool ButtonModeComponent::HitTest(int mouseX, int mouseY) const {
    return (mouseX >= posX && mouseX <= (posX + width) && mouseY >= posY && mouseY <= (posY + height));
}

void ButtonModeComponent::OnClick() {
    MessageBox(NULL, L"Button Clicked!", L"Notification", MB_OK);
}

void ButtonModeComponent::MoveInnerRectangle(bool isOn) {
	this->isOn = isOn;
}
