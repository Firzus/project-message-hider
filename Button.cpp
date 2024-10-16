#include "Button.h"

// Constructeur pour initialiser le bouton
Button::Button(HWND parent, int x, int y, int width, int height, int buttonId, bool primaryStyle) {
    hWndParent = parent;
    rect = { x, y, x + width, y + height };
    id = buttonId;
    isPrimary = primaryStyle;

    // Déterminer les couleurs initiales
    UpdateColors();

    // Créer le bouton (invisible, car nous allons le dessiner nous-mêmes)
    hButton = CreateWindowW(
        L"STATIC",  // Utiliser STATIC pour dessiner nous-mêmes
        NULL,       // Pas de texte par défaut
        WS_CHILD | WS_VISIBLE,  // Visible et enfant de la fenêtre parent
        rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
        hWndParent, (HMENU)(INT_PTR)id, (HINSTANCE)GetWindowLongPtr(hWndParent, GWLP_HINSTANCE), NULL
    );
}

Button::~Button() {}

// Méthode pour changer les couleurs
void Button::UpdateColors() {
    if (isPrimary) {
        bgColor = RGB(2, 6, 23);         // Couleur de fond pour le style primaire
        textColor = RGB(255, 255, 255);  // Couleur du texte pour le style primaire
    }
    else {
        bgColor = RGB(241, 245, 249);    // Couleur de fond pour le style secondaire
        textColor = RGB(0, 0, 0);        // Couleur du texte pour le style secondaire
    }
}

// Dessiner le bouton
void Button::Draw(HDC hdc) {
    // Dessiner le fond
    HBRUSH hBrush = CreateSolidBrush(bgColor);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);

    // Dessiner le texte
    SetTextColor(hdc, textColor);
    SetBkMode(hdc, TRANSPARENT);

    // Dessiner le texte centré
    DrawText(hdc, (isPrimary ? L"Primary Button" : L"Secondary Button"), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void Button::OnClick() const {
    MessageBox(hWndParent, L"Button clicked!", L"Button Clicked", MB_OK);
}

// Fonction pour changer le style du bouton
void Button::SetStyle(bool primaryStyle) {
    isPrimary = primaryStyle;

    // Mettre à jour les couleurs
    UpdateColors();

    InvalidateRect(hButton, NULL, TRUE);
}

HWND Button::GetHandle() const {
    return hButton;
}