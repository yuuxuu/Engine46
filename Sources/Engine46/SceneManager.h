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

	class SceneManager {
	private:
		std::vector<std::unique_ptr<CSceneBase>>	m_pSceneList;

	public:
		SceneManager();
		~SceneManager();

		CSceneBase* CreateScene(SceneType sceneType);

		bool SaveScene();
		bool LoadScene();

		void AddSceneToSceneList(std::unique_ptr<CSceneBase>& pScene) { m_pSceneList.emplace_back(move(pScene)); }

	private:
		void ConnectScene();
	};

} // namespace

#endif