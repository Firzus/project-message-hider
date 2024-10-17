#include "TextComponent.h"

TextComponent::TextComponent(HFONT initiaFont, const std::wstring& initialText, int posX, int posY, int initialFrameWidth, COLORREF textColor)
    : posX(posX), posY(posY), textColor(textColor), text(initialText), frameWidth(initialFrameWidth), font(initiaFont) {}

TextComponent::~TextComponent() {}

void TextComponent::Draw(HDC hdc)
{
    // S�lectionner la police dans le contexte de p�riph�rique
    HFONT oldFont = (HFONT)SelectObject(hdc, font);
    if (oldFont == HGDI_ERROR) {
        // Si la s�lection �choue, ne pas dessiner
        return;
    }

    // D�finir la couleur du texte
    SetTextColor(hdc, textColor);
    SetBkMode(hdc, TRANSPARENT); // Mode de fond transparent

    // Calculer la taille du texte
    SIZE textSize;
    GetTextExtentPoint32(hdc, text.c_str(), (int)text.length(), &textSize);

    // Rectangle pour le texte
    RECT rect;
    SetRect(&rect, posX, posY, posX + frameWidth, posY + textSize.cy); // Utiliser la position et la largeur du cadre

    // Dessiner le texte avec DT_WORDBREAK pour g�rer le retour � la ligne
    DrawText(hdc, text.c_str(), -1, &rect, DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP);

    // Restaurer la police pr�c�dente
    SelectObject(hdc, oldFont);
}

void TextComponent::SetColor(HWND hWnd, COLORREF newColor)
{
    textColor = newColor;
}

void TextComponent::SetText(HWND hWnd, const std::wstring& newText)
{
    text = newText;
}

void TextComponent::DeleteText(HWND hWnd)
{
    text.clear();
}
