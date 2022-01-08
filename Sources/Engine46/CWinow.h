/**
 * @file CWinowManager.h
 * @brief
 * @author 木村優
 * @date 2019/09/28
 */

#pragma once

#ifndef _CWINOW_H_ 
#define _CWINOW_H_

#include "math.h"

namespace Engine46 {

    class CWindow {
    private:
        HWND        m_hwnd;
        HINSTANCE   m_hInstance;
        const char* m_className;

        RECT        m_windowSize;
        RECT        m_clientSize;

    public:
        CWindow();
        ~CWindow();

        bool Initialize(HINSTANCE hInstance, const char* className, const char* titleName);

        void ChangeSizeWindow(const int posx, const int posy, const int width, const int height);

        void ScreenShotWindow();

        HWND GetHwnd() const { return m_hwnd; }

        HINSTANCE GethInstance() const { return m_hInstance; }

        RECT GetWindowSize() const { return m_windowSize; }

        RECT GetClientSize() const { return m_clientSize; }
    private:
        static LRESULT CALLBACK	WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

        bool GetBuffer(std::unique_ptr<BYTE[]>& pBuf);

    };

} // namespace

#endif
