#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(int x, int y, int width, int height, const wchar_t* text, int id)
	: posX(x), posY(y), width(width), height(height), text(text), id(id), isPrimary(true)
{
	bgColorLight = RGB(0, 120, 215);
	bgColorDark = RGB(0, 99, 177);
	textColorLight = RGB(255, 255, 255);
	textColorDark = RGB(255, 255, 255);
}

void ButtonComponent::Draw(HDC hdc) {
    // Dessiner le bouton
    HBRUSH hBrush = CreateSolidBrush(isPrimary ? bgColorLight : bgColorDark); // Couleur de remplissage
    RECT rect = { posX, posY, posX + width, posY + height };
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);

    // Ajouter une bordure si isPrimary est vrai
    //if (isPrimary) {
    //    HBRUSH hBorderBrush = CreateSolidBrush(borderColor);
    //    FrameRect(hdc, &rect, hBorderBrush);
    //    DeleteObject(hBorderBrush);
    //}

    // Dessiner le texte
    SetTextColor(hdc, isPrimary ? textColorLight : textColorDark); // Couleur du texte
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