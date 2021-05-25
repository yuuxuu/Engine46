/**
 * @file CGameSystem.h
 * @brief
 * @author 木村優
 * @date 2018/12/19
 */

#pragma once

#ifndef _CGAME_SYSTEM_H_
#define _CGAME_SYSTEM_H_

#pragma comment(lib, "GraphicsAPI.lib")

namespace Engine46 {

	// 前方宣言
	class CWindow;
	class CRendererBase;
	class CShaderManager;
	class CActorManager;
	class CMeshManager;
	class CMaterialManager;
	class CTextureManager;
	class CSceneManager;
	class CInput;

	class CGameSystem {
	private:
		std::thread							m_gameSystemThread;
		HANDLE								m_hGame;

		std::unique_ptr<CWindow>			m_pMainWindow;

		std::unique_ptr<CRendererBase>		m_pRenderer;

		std::unique_ptr<CShaderManager>		m_pShaderManager;

		std::unique_ptr<CActorManager>		m_pActorManager;

		std::unique_ptr<CMeshManager>		m_pMeshManager;

		std::unique_ptr<CMaterialManager>	m_pMaterialManager;

		std::unique_ptr<CTextureManager>	m_pTextureManager;

		std::unique_ptr<CSceneManager>		m_pSceneManager;

		std::unique_ptr<CInput>				m_pInput;

	public:
		CGameSystem();
		~CGameSystem();

		bool				Initialize(HINSTANCE hInstance);
		void				Finalize();

		void				Loop();
		void				Update();
		void				Draw();
	};

} // namespace

#endif
