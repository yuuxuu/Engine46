/**
 * @file CScene.h
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#ifndef _CSCENE_H_
#define _CSCENE_H_

#pragma once

#include "DataRecord.h"

namespace Engine46 {

	enum class SceneType {
		TITLE,
		GAME,
		RESULT,
		LOADING,
		NONE,
	};

	class CSceneBase {
	protected:
		static std::vector<DATARECORD>	m_dataRecordsVec;

		std::vector<STR_DATARECORD>		m_strDataRecordsVec;

		CSceneBase*						pParentScene;
		std::list<CSceneBase*>			pChiledSceneList;

		SceneType						m_SceneType;
		std::unique_ptr<char[]>			m_SceneName;

		CSceneBase(const SceneType sceneType, const char* sceneName);
		virtual ~CSceneBase();

		void ConnectParentScene(CSceneBase* pParentScene) { this->pParentScene = pParentScene; };
		CSceneBase* GetParentScene() const { return pParentScene; }

		void AddChiledSceneList(CSceneBase* pChiledScene) { pChiledSceneList.emplace_back(pChiledScene); }
		std::list<CSceneBase*> GetChildSceneList() const { return pChiledSceneList; }

		virtual void InitializeScene();
		virtual void UpdateScene();
		virtual void DrawScene();

		virtual bool SaveScene(std::ofstream& ofs);
		virtual bool LoadScene(std::ifstream& ifs);
	};

} // namespace

#endif
