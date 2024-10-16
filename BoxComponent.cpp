#include "BoxComponent.h"

BoxComponent::BoxComponent(int posX, int posY, int colorID) :
	posX(posX), posY(posY), colorID(colorID) {}

BoxComponent::~BoxComponent() {}

void BoxComponent::Draw(HDC hdc, int width, int height, COLORREF fillColor) const
{
	// Créer un pinceau avec la couleur de remplissage
	HBRUSH hBrush = CreateSolidBrush(fillColor);

	// Sélectionner le pinceau dans le contexte de périphérique
	HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);

	// Pour désactiver le contour (bordure) 
	HPEN hNullPen = CreatePen(PS_NULL, 0, 0);
	HGDIOBJ hOldPen = SelectObject(hdc, hNullPen);

	// Dessiner le rectangle (rempli)
	Rectangle(hdc, posX, posY, posX + width, posY + height);

	// Restaurer l'ancien pinceau et stylo
	SelectObject(hdc, hOldBrush);
	SelectObject(hdc, hOldPen);

	// Supprimer le pinceau et le stylo
	DeleteObject(hBrush);
	DeleteObject(hNullPen);
}

void BoxComponent::SetColor(HWND hWnd, int newColorID) {
	colorID = newColorID;
}