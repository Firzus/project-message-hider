#include "TextComponent.h"

TextComponent::TextComponent(HFONT initiaFont, const std::wstring& initialText, int posX, int posY, int initialFrameWidth, int colorID)
    : posX(posX), posY(posY), colorID(colorID), text(initialText), frameWidth(initialFrameWidth), font(initiaFont) {}

TextComponent::~TextComponent() {}

void TextComponent::Draw(HDC hdc, COLORREF textColor)
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

void TextComponent::SetColor(HWND hWnd, int newColorID)
{
    colorID = newColorID;
}

void TextComponent::SetText(HWND hWnd, const std::wstring& newText)
{
    text = newText;
}

void TextComponent::DeleteText(HWND hWnd)
{
    text.clear();
}
