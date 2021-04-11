/**
 * @file utility.cpp
 * @brief
 * @author –Ø‘º—D
 * @date 2021/04/10
 */

#include "utility.h"

namespace Engine46 {
	// BMPƒtƒ@ƒCƒ‹‚Ì•Û‘¶
	void SaveToBmpFile(BITMAPINFOHEADER m_bmpInfo, LPCVOID pBuffer, std::string fileName) {

		HANDLE handle = CreateFile(fileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (handle != INVALID_HANDLE_VALUE) {

			BITMAPFILEHEADER bmpHead;
			ZeroMemory(&bmpHead, sizeof(bmpHead));
			DWORD writeSize;

			bmpHead.bfType = ('M' << 8) | 'B';
			bmpHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_bmpInfo.biSizeImage;
			bmpHead.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

			WriteFile(handle, &bmpHead, sizeof(BITMAPFILEHEADER), &writeSize, NULL);
			WriteFile(handle, &m_bmpInfo, sizeof(BITMAPINFOHEADER), &writeSize, NULL);
			WriteFile(handle, pBuffer, m_bmpInfo.biSizeImage, &writeSize, NULL);
		}
		else {
			MessageBox(NULL, "BMPƒtƒ@ƒCƒ‹‚Ì•Û‘¶:Ž¸”s", "MessageBox", MB_OK);
		}
		CloseHandle(handle);
	}

	// WStringŒ^‚É•ÏŠ·
	std::wstring StringConvertToWString(std::string str) {
		std::wstring result;
		size_t size = 0;
		std::unique_ptr<wchar_t[]> wchar = std::make_unique<wchar_t[]>(str.size() + 1);

		mbstowcs_s(&size, wchar.get(), str.size() + 1, str.c_str(), _TRUNCATE);

		result = wchar.get();

		return result;
	}
} // namespace