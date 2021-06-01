/**
 * @file CSceneManager.h
 * @brief
 * @author 木村優
 * @date 2021/05/01
 */

#pragma once

#ifndef _CSCENE_MANAGER_H_
#define _CSCENE_MANAGER_H_

namespace Engine46 {

	// 前方宣言
	class CSceneBase;

	class CSceneManager {
	private:
		std::vector<std::unique_ptr<CSceneBase>>	m_pVecScene;
		CSceneBase*									pRootScene;

	public:
		CSceneManager();
		~CSceneManager();

		CSceneBase* CreateScene(int id);

		void AddSceneToVec(std::unique_ptr<CSceneBase>& pScene) { m_pVecScene.emplace_back(std::move(pScene)); }
		
		CSceneBase* GetRootScene() const { return pRootScene; };

		bool SaveScene();
		bool LoadScene();

	private:
		void ConnectScene();
	};

} // namespace

#endif