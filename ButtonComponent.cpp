#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(int x, int y, int width, int height, const wchar_t* text, int id)
    : posX(x), posY(y), width(width), height(height), text(text), id(id)  {}

void ButtonComponent::Draw(HDC hdc, COLORREF bgColor, COLORREF textColor) {
    // Dessiner le bouton
    HBRUSH hBrush = CreateSolidBrush(bgColor); // Couleur de remplissage
    RECT rect = { posX, posY, posX + width, posY + height };
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);

    // Dessiner le texte
    SetTextColor(hdc, textColor); // Couleur du texte
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, text, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

bool ButtonComponent::HitTest(int mouseX, int mouseY) const {
    return (mouseX >= posX && mouseX <= (posX + width) && mouseY >= posY && mouseY <= (posY + height));
}

void ButtonComponent::OnClick() {
    MessageBox(NULL, L"Button Clicked!", L"Notification", MB_OK);
}

void ButtonComponent::DeleteButton(HWND hWnd) {
    InvalidateRect(hWnd, NULL, TRUE);
}