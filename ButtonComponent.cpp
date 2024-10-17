#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(int x, int y, int width, int height, int id, bool isPrimary)
	: posX(x), posY(y), width(width), height(height), id(id), isPrimary(isPrimary)
{
	colorLight = RGB(248, 250, 252);
	colorDark = RGB(15, 23, 42);
}

void ButtonComponent::Draw(HDC hdc, const wchar_t* text) {
    // Dessiner le bouton
    HBRUSH hBrush = CreateSolidBrush(isPrimary ? colorDark : colorLight); // Couleur de remplissage
    RECT rect = { posX, posY, posX + width, posY + height };
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);

    // Ajouter une bordure si isPrimary est vrai
    if (!isPrimary) {
        HBRUSH hBorderBrush = CreateSolidBrush(colorDark);
        FrameRect(hdc, &rect, hBorderBrush);
        DeleteObject(hBorderBrush);
    }

    // Dessiner le texte
    SetTextColor(hdc, isPrimary ? colorLight : colorDark);
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

void ButtonComponent::SetStyle(HWND hWnd, bool isPrimary)
{
	this->isPrimary = isPrimary;
    InvalidateRect(hWnd, NULL, TRUE);
}
