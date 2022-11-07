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

    // char型からwstring型に変換
    void CharConvertToWchar(const char* srcStr, std::wstring& destStr) {
        size_t size = strlen(srcStr) + 1;
        destStr.reserve(size);

        std::wstring outStr;
        outStr.reserve(size);

        mbstowcs_s(&size, outStr.data(), size, srcStr, _TRUNCATE);

        destStr = outStr.c_str();
    }

    // wstring型からstring型に変換
    void WStringConvertToStrig(const std::wstring& srcStr, std::string& destStr) {
        size_t size = srcStr.size() + 1;
        destStr.reserve(size);

        std::string outStr;
        outStr.reserve(size);

        wcstombs_s(&size, outStr.data(), size, srcStr.c_str(), _TRUNCATE);

        destStr = outStr.c_str();
    }

} // namespace