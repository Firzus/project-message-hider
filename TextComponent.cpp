#include "TextComponent.h"

TextComponent::TextComponent(HDC hdc, const std::wstring& text, int posX, int posY, HFONT font, COLORREF textColor, int frameWidth)
{
    // S�lectionner la police dans le contexte de p�riph�rique
    HFONT oldFont = (HFONT)SelectObject(hdc, font);

    // D�finir la couleur du texte
    SetTextColor(hdc, textColor);
    SetBkMode(hdc, TRANSPARENT); // Mode de fond transparent

    // Calculer la taille du texte
    SIZE textSize;
    GetTextExtentPoint32(hdc, text.c_str(), text.length(), &textSize);

    // Rectangle pour le texte
    RECT rect;
    SetRect(&rect, posX, posY, posX + frameWidth, posY + textSize.cy); // Utiliser la position et la largeur du cadre

    // Dessiner le texte avec DT_WORDBREAK pour g�rer le retour � la ligne
    DrawText(hdc, text.c_str(), -1, &rect, DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP);

    // Restauration de la police pr�c�dente
    SelectObject(hdc, oldFont);
}

TextComponent::~TextComponent() {}