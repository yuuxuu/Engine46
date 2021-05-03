/**
 * @file CWinowManager.h
 * @brief
 * @author 木村優
 * @date 2019/09/28
 */

#ifndef _CWINOW_H_ 
#define _CWINOW_H_

#pragma once

#include "math.h"

namespace Engine46 {

	class CWindow {
	private:
		HWND		m_hwnd;
		HINSTANCE	m_hInstance;
		const char* m_className;

		RECT		m_windowSize;
		RECT		m_clientSize;

	public:
		CWindow();
		~CWindow();

		static LRESULT CALLBACK	WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		bool	Initialize(HINSTANCE hInstance, const char* className, const char* titleName);

		void	ChangeSizeWindow(const int posx, const int posy, const int width, const int height);
				
		void	ScreenShotWindow();
				
		bool	GetBuffer(std::unique_ptr<BYTE[]>& pBuf);
				
		HWND	GetHwnd() const { return m_hwnd; }
			 
		RECT	GetWindowWidth() const { return m_windowSize; }

		RECT	GetClientSize() const { return m_clientSize; }
	};

} // namespace

#endif
