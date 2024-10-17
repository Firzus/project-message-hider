#include "TextFieldComponent.h"

TextFieldComponent::TextFieldComponent(HWND parentHandle, HINSTANCE hInstance, int x, int y, int width, int height) 
	: hParent(parentHandle), hInst(hInstance), _x(x), _y(y), _width(width), _height(height)
{
	hTextField = CreateWindowEx(0, L"EDIT", L"Enter your text here",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
		x, y, width, height, parentHandle, nullptr, hInstance, nullptr);
	SendMessage(hTextField, EM_SETLIMITTEXT, maxChars, 0);
	DrawUnderline();
	UpdateCharCount();
}

TextFieldComponent::~TextFieldComponent()
{
	DestroyWindow(hTextField);
}

std::wstring TextFieldComponent::GetText()
{
	int length = GetWindowTextLength(hTextField);
	wchar_t* buffer = new wchar_t[length + 1];

	GetWindowText(hTextField, buffer, length + 1);
	std::wstring text(buffer);
	delete[] buffer;
	return text;
}

void TextFieldComponent::DrawUnderline()
{
}

std::wstring TextFieldComponent::UpdateCharCount()
{
	int currentLength = GetWindowTextLength(hTextField);
	int remainingChars = maxChars - currentLength;

	std::wstring charCountMessage = L"Text (" + std::to_wstring(remainingChars) + L") left";
	return charCountMessage;
}
