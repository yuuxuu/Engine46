/**
 * @file CScene.h
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#pragma once

#ifndef _CSCENE_H_
#define _CSCENE_H_

#include "IObject.h"
#include "CDataRecord.h"

namespace Engine46 {

	// 前方宣言
	class CActorBase;

	class CSceneBase : public IObject {
	protected:
		std::vector<std::unique_ptr<CDataRecordBase>>	vecDataRecords;

		CActorBase*										pRootActor;

		CSceneBase*										pParentScene;
		int												m_parentSceneID;

		std::list<CSceneBase*>							pChiledSceneList;
		std::vector<int>								m_chiledSceneIDList;

		int												m_SceneID;

		std::unique_ptr<char[]>							m_SceneName;

	public:
		CSceneBase();
		explicit CSceneBase(const char* sceneName);
		virtual ~CSceneBase();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Draw() override;

		virtual bool Save(std::ofstream& ofs) override;
		virtual bool Load(std::ifstream& ifs) override;

		void ConnectParentScene(CSceneBase* pParentScene);

		CSceneBase* GetParentScene() const { return pParentScene; }
		int GetParentSceneID() const { return m_parentSceneID; }

		void AddChiledSceneList(CSceneBase* pChiledScene);

		std::list<CSceneBase*> GetChildSceneList() const { return pChiledSceneList; }
		std::vector<int> GetChiledSceneIDList() const { return m_chiledSceneIDList; }

		void SetRootActor(CActorBase* pRootActor) { this->pRootActor = pRootActor; }
	};

} // namespace

#endif
