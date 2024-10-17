#include "BoxComponent.h"

BoxComponent::BoxComponent(int posX, int posY, COLORREF initialFillColor) :
	posX(posX), posY(posY), fillColor(initialFillColor) {}

BoxComponent::~BoxComponent() {}

void BoxComponent::Draw(HDC hdc, int width, int height)
{
	// Cr�er un pinceau avec la couleur de remplissage
	HBRUSH hBrush = CreateSolidBrush(fillColor);

	// S�lectionner le pinceau dans le contexte de p�riph�rique
	HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);

	// Pour d�sactiver le contour (bordure) 
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

void BoxComponent::SetColor(HWND hWnd, COLORREF newColor) {
	fillColor = newColor;
}