/**
 * @file CGameSystem.h
 * @brief
 * @author 木村優
 * @date 2018/12/19
 */
#pragma once

#ifndef _CGAME_SYSTEM_MANAGER_H_
#define _CGAME_SYSTEM_MANAGER_H_

#include "CWinow.h"

#pragma comment(lib, "renderer.lib")
#pragma comment(lib, "shader.lib")

namespace Engine46 {

	class CDX11Renderer;

	class CGameSystem {
	private:
		std::thread						m_gameSystemThread;
		HANDLE							m_hGame;

		std::unique_ptr<CWindow>		m_mainWindow;

		std::unique_ptr<CDX11Renderer>	m_pRenderer;

		int								m_fps;
		int								m_wfps;

		int								m_nowTime;
		int								m_oldTime;

	public:
		CGameSystem();
		~CGameSystem();

		bool				Initialize(HINSTANCE hInstance);
		void				Finalize();

		void				Loop();
		void				Update();
		void				Draw();
		
		void				MeasFPS();

		int					GetFPS() const { return m_fps; }
	};

} // namespace

#endif
