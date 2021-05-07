/**
 * @file CGameSystem.h
 * @brief
 * @author 木村優
 * @date 2018/12/19
 */

#pragma once

#ifndef _CGAME_SYSTEM_H_
#define _CGAME_SYSTEM_H_

#pragma comment(lib, "Renderer.lib")
#pragma comment(lib, "Scene.lib")

namespace Engine46 {

	// 前方宣言
	class CWindow;
	class CDX11Renderer;
	class CSceneManager;

	class CGameSystem {
	private:
		std::thread						m_gameSystemThread;
		HANDLE							m_hGame;

		std::unique_ptr<CWindow>		m_pMainWindow;

		std::unique_ptr<CDX11Renderer>	m_pDX11Renderer;

		std::unique_ptr<CSceneManager>	m_pSceneManager;

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
