/**
 * @file utility.cpp
 * @brief
 * @author 木村優
 * @date 2021/04/10
 */

#include "utility.h"

namespace Engine46 {
	// BMPファイルの保存
	void SaveToBmpFile(BITMAPINFOHEADER m_bmpInfo, LPCVOID pStr, const char* fileName) {

		HANDLE handle = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (handle != INVALID_HANDLE_VALUE) {

			BITMAPFILEHEADER bmpHead;
			ZeroMemory(&bmpHead, sizeof(bmpHead));
			DWORD writeSize;

			bmpHead.bfType = ('M' << 8) | 'B';
			bmpHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_bmpInfo.biSizeImage;
			bmpHead.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

			WriteFile(handle, &bmpHead, sizeof(BITMAPFILEHEADER), &writeSize, NULL);
			WriteFile(handle, &m_bmpInfo, sizeof(BITMAPINFOHEADER), &writeSize, NULL);
			WriteFile(handle, pStr, m_bmpInfo.biSizeImage, &writeSize, NULL);
		}
		else {
			MessageBox(NULL, "BMPファイルの保存:失敗", "MessageBox", MB_OK);
		}
		CloseHandle(handle);
	}

	// WString型に変換
	std::wstring StringConvertToWString(std::string str) {
		std::wstring result;
		size_t size = 0;
		std::unique_ptr<wchar_t[]> wchar = std::make_unique<wchar_t[]>(str.size() + 1);

		mbstowcs_s(&size, wchar.get(), str.size() + 1, str.c_str(), _TRUNCATE);

		result = wchar.get();

		return result;
	}
} // namespace