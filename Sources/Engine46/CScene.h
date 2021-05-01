/**
 * @file CScene.h
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#ifndef _CSCENE_H_
#define _CSCENE_H_

#pragma once

#include "ObjectInterface.h"

namespace Engine46 {

	enum class SceneType {
		TITLE,
		GAME,
		RESULT,
		LOADING,
		NONE,
	};

	class CSceneBase : public ObjectInterface {
	protected:
		CSceneBase*						pParentScene;
		std::list<CSceneBase*>			pChiledSceneList;

		SceneType						m_SceneType;

		int								m_SceneID;

		std::unique_ptr<char[]>			m_SceneName;

	public:
		CSceneBase(const SceneType sceneType, const char* sceneName);
		virtual ~CSceneBase();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Draw() override;

		virtual bool Save(std::ofstream& ofs) override;
		virtual bool Load(std::ifstream& ifs) override;

		void ConnectParentScene(CSceneBase* pParentScene) { this->pParentScene = pParentScene; };
		CSceneBase* GetParentScene() const { return pParentScene; }

		void AddChiledSceneList(CSceneBase* pChiledScene) { pChiledSceneList.emplace_back(pChiledScene); }
		std::list<CSceneBase*> GetChildSceneList() const { return pChiledSceneList; }
	};

} // namespace

#endif
