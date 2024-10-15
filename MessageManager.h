#pragma once
#include "framework.h"

class MessageManager
{
public:
	bool HideMessage(const std::wstring& ImagePath, const std::string& Message, HDC hdc);
	std::string GetMessage(const std::wstring& ImagePath);
private:
	std::vector<int> ConvertStringToBits(const std::string& Message);
	bool SaveMessage(HBITMAP HBitmap, BITMAP& Bmp, const std::vector<int>& MessageBits);
	HBITMAP GetBitMapFromPng(LPCWSTR filePath, HDC hdc);
	bool SaveBitmapAsPng(HBITMAP hBitmap, const std::wstring& outputPath, HDC hdc);
};
