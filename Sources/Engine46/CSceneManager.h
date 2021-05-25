/**
 * @file CSceneManager.h
 * @brief
 * @author 木村優
 * @date 2021/05/01
 */

#pragma once

#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

namespace Engine46 {

	// 前方宣言
	class CActorBase;
	class CSceneBase;

	class CSceneManager {
	private:
		std::vector<std::unique_ptr<CSceneBase>>	m_pVecScene;
		CSceneBase*									pRootScene;

	public:
		CSceneManager();
		~CSceneManager();

		bool Initialize(CActorBase* pRootActor);

		CSceneBase* CreateScene(int id);

		bool SaveScene();
		bool LoadScene();

		void AddSceneToVec(std::unique_ptr<CSceneBase>& pScene) { m_pVecScene.emplace_back(move(pScene)); }

		CSceneBase* GetRootScene() const { return pRootScene; };

	private:
		void ConnectScene();
	};

} // namespace

#endif