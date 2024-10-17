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

// Méthode pour obtenir une couleur par index
COLORREF Theme::GetColor(int index) {
    if (colorPalette.find(index) != colorPalette.end()) {
        return colorPalette[index];
    }
    else {
        // Retourner une valeur par défaut (rouge) si l'index n'existe pas pour debug
        return RGB(255, 10, 10);
    }
}

void Theme::ToggleMode() {
    // Créer une copie de la palette actuelle pour faciliter l'inversion
    std::map<int, COLORREF> tempPalette = colorPalette;

    // Inverser les couleurs en échangeant symétriquement les valeurs
    colorPalette[50] = tempPalette[950];
    colorPalette[100] = tempPalette[900];
    colorPalette[200] = tempPalette[800];
    colorPalette[300] = tempPalette[700];
    colorPalette[400] = tempPalette[600];
    colorPalette[500] = tempPalette[500];
    colorPalette[600] = tempPalette[400];
    colorPalette[700] = tempPalette[300];
    colorPalette[800] = tempPalette[200];
    colorPalette[900] = tempPalette[100];
    colorPalette[950] = tempPalette[50];
}