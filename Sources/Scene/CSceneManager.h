/**
 * @file CSceneManager.h
 * @brief
 * @author 木村優
 * @date 2021/05/01
 */

#pragma once

#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "CScene.h"

namespace Engine46 {

	// 前方宣言
	class CDX11Renderer;

	class CSceneManager {
	private:
		std::vector<std::unique_ptr<CSceneBase>>	m_pVecScene;
		CSceneBase*									pRootScene;

		CDX11Renderer*								pDX11Renderer;

		std::unique_ptr<CShaderManager>				m_pShaderManager;
		std::unique_ptr<CActorManager>				m_pActorManager;

		void ConnectScene();

	public:
		CSceneManager(CDX11Renderer* pRenderer);
		~CSceneManager();

		bool Initialize();

		CSceneBase* CreateScene(int id);

		bool SaveScene();
		bool LoadScene();

		void AddSceneToSceneVec(std::unique_ptr<CSceneBase>& pScene) { m_pVecScene.emplace_back(move(pScene)); }

		CSceneBase* GetRootScene() const { return pRootScene; };
	};

} // namespace

#endif