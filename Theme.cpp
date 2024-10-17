#include "Theme.h"

Theme::Theme()
{
    colorPalette[50] = RGB(248, 250, 252);   // #F8FAFC
    colorPalette[100] = RGB(241, 245, 249);  // #F1F5F9
    colorPalette[200] = RGB(226, 232, 240);  // #E2E8F0
    colorPalette[300] = RGB(203, 213, 225);  // #CBD5E1
    colorPalette[400] = RGB(148, 163, 184);  // #94A3B8
    colorPalette[500] = RGB(100, 116, 139);  // #64748B
    colorPalette[600] = RGB(71, 85, 105);    // #475569
    colorPalette[700] = RGB(51, 65, 85);     // #334155
    colorPalette[800] = RGB(30, 41, 59);     // #1E293B
    colorPalette[900] = RGB(15, 23, 42);     // #0F172A
    colorPalette[950] = RGB(2, 6, 23);       // #020617
}

Theme::~Theme() {}

// Méthode pour ajouter une nouvelle couleur avec un index
void Theme::AddColor(int index, COLORREF color) {
    colorPalette[index] = color;
}

// Méthode pour obtenir une couleur par index
COLORREF Theme::GetColor(int index) {
    if (colorPalette.find(index) != colorPalette.end()) {
        return colorPalette[index];
    }
    else {
        // Retourner une valeur par défaut (rouge) si l'index n'existe pas
        return RGB(255, 10, 10);
    }
}

// Méthode pour obtenir une couleur par index sous forme hexadécimale (optionnel)
std::string Theme::GetColorHex(int index) {
    COLORREF color = GetColor(index);
    char hexColor[8];
    sprintf_s(hexColor, "#%02X%02X%02X", GetRValue(color), GetGValue(color), GetBValue(color));
    return std::string(hexColor);
}