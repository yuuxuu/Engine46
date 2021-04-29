/**
 * @file CWindowManager.h
 * @brief
 * @author 木村優
 * @date 2019/09/26
 */

#ifndef _CWINDOW_MANAGER_H_ 
#define _CWINDOW_MANAGER_H_

#pragma once

#include	"CWinow.h"

namespace Engine46 {

	class CWindowManager {
	private:
		static CWindowManager*					m_pCWinowManager;

		HINSTANCE								m_hInstance;

		std::vector<std::unique_ptr<CWindow>>	m_pVecCWindow;

		int										m_WinMode;

		CWindowManager();
		~CWindowManager();
	public:
		void					Update();

		bool					CreateCWindow(HINSTANCE hInstance, std::unique_ptr<CWindow> pWindow);

		HINSTANCE				GethInstance() const { return m_hInstance; }
		void					SethInstance(HINSTANCE hInstance) { m_hInstance = hInstance; }

		void					SetWinMode(int winmode) { m_WinMode = winmode; }

		static void				Create();
		static void				Release();
		static CWindowManager*	Get() { return m_pCWinowManager; };
	};

} // namespace

#endif