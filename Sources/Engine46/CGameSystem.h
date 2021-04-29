/**
 * @file CGameSystem.h
 * @brief
 * @author �ؑ��D
 * @date 2018/12/19
 */
#pragma once

#ifndef _CGAME_SYSTEM_MANAGER_H_
#define _CGAME_SYSTEM_MANAGER_H_

#include "CWinow.h"

namespace Engine46 {

	class CGameSystem {
	private:
		std::thread					m_gameSystemThread;
		HANDLE						m_hGame;

		std::unique_ptr<CWindow>	m_mainWindow;

		int							m_fps;
		int							m_wfps;

		int							m_nowTime;
		int							m_oldTime;

	public:
		CGameSystem();
		~CGameSystem();

		bool				GameSystemInit(HINSTANCE hInstance);

		void				GameSystemLoop();
		void				GameSystemUpdate();
		void				GameSystemDraw();
		void				GameSystemExit();
		
		void				MeasFPS();

		int					GetFPS() const { return m_fps; }
	};

} // namespace

#endif