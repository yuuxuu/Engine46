/**
 * @file CWinowManager.h
 * @brief
 * @author 木村優
 * @date 2019/09/28
 */

#ifndef _CWINOW_H_ 
#define _CWINOW_H_

#pragma once

#include "main.h"

namespace Engine46 {

	class CWindow {
	private:
		HWND		m_hwnd;
		WNDCLASSEX	m_wcex;

		const char* m_pWindowClassName;
		const char* m_pTitleName;

		int			m_windowWidth;
		int			m_windowHeight;
			 
		int			m_clientWidth;
		int			m_clientHeight;

		bool		m_onScreenShot;
		bool		m_onFullScreen;

	public:
		CWindow(const char* windowName, const char* titleName);
		~CWindow();

		static LRESULT CALLBACK	WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		bool	InitInstance(HINSTANCE hInstance);

		bool	InitWindow(HINSTANCE hInstance, int nWinMode);

		void	ChangeSizeWindow(const int posx, const int posy, const int width, const int height);
				
		void	ScreenShotWindow();
				
		BYTE*	GetBuffer();
				
		HWND	GetHwnd() const { return m_hwnd; }
			 
		int		GetWindowWidth() const { return m_windowWidth; }
		int		GetWindowHeight() const { return m_windowHeight; }
			 
		int		GetClientWidth() const { return m_clientWidth; }
		int		GetClientHeight() const { return m_clientHeight; }
			 
		bool	GetOnScreenShot() const { return m_onScreenShot; }
		void	SetOnScreenShot(bool flag) { m_onScreenShot = flag; }
			 
		bool	GetOnFullScreen() const { return m_onFullScreen; }
		void	SetOnFullScreen(bool flag) { m_onFullScreen = flag; }
	};

} // namespace

#endif
