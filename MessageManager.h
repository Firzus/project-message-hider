#pragma once
#include "framework.h"

class MessageManager
{
public:
	bool HideMessage(const std::wstring& ImagePath, const std::wstring& Message, HDC hdc);
	std::wstring GetMessage(const std::wstring& ImagePath, HDC hdc);
private:
	std::vector<int> ConvertStringToBits(const std::wstring& Message);
	bool SaveMessage(HBITMAP HBitmap, BITMAP& Bmp, const std::vector<int>& MessageBits);
	HBITMAP GetBitMapFromPng(LPCWSTR filePath, HDC hdc);
	bool SaveBitmapAsPng(HBITMAP hBitmap, const std::wstring& outputPath, HDC hdc);
	char GetCharFromByte(const std::vector<int>& BitsMap, int Index);
	std::wstring SaveMessageInFile(std::wstring embededMessage, std::wstring fileName);
};
