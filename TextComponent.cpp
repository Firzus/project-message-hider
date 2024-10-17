#include "TextComponent.h"

TextComponent::TextComponent(int posX, int posY, COLORREF textColor)
    : posX(posX), posY(posY), textColor(textColor) {}

TextComponent::~TextComponent() {}

void TextComponent::Draw(HDC hdc, HFONT font, const std::wstring& text, int frameWidth)
{
    // Sélectionner la police dans le contexte de périphérique
    HFONT oldFont = (HFONT)SelectObject(hdc, font);
    if (oldFont == HGDI_ERROR) {
        // Si la sélection échoue, ne pas dessiner
        return;
    }

    // Définir la couleur du texte
    SetTextColor(hdc, textColor);
    SetBkMode(hdc, TRANSPARENT); // Mode de fond transparent

    // Calculer la taille du texte
    SIZE textSize;
    GetTextExtentPoint32(hdc, text.c_str(), (int)text.length(), &textSize);

    // Rectangle pour le texte
    RECT rect;
    SetRect(&rect, posX, posY, posX + frameWidth, posY + textSize.cy); // Utiliser la position et la largeur du cadre

    // Dessiner le texte avec DT_WORDBREAK pour gérer le retour à la ligne
    DrawText(hdc, text.c_str(), -1, &rect, DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP);

    // Restaurer la police précédente
    SelectObject(hdc, oldFont);
}

void TextComponent::SetColor(HWND hWnd, COLORREF newColor)
{
	textColor = newColor;
	InvalidateRect(hWnd, nullptr, TRUE);
}

void TextComponent::DeleteText(HWND hWnd)
{
	InvalidateRect(hWnd, nullptr, TRUE);
}
