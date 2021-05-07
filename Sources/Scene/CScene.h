/**
 * @file CScene.h
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#pragma once

#ifndef _CSCENE_H_
#define _CSCENE_H_

#include "../Engine46/IObject.h"
#include "../Engine46/CDataRecord.h"
#include "../Shader/CShaderManager.h"
#include "../Actor/CActorManager.h"

#pragma comment(lib, "Shader.lib")
#pragma comment(lib, "Actor.lib")

namespace Engine46 {

	class CSceneBase : public IObject {
	protected:
		std::vector<std::unique_ptr<CDataRecordBase>>	vecDataRecords;

		CShaderManager*									pShaderManager;
		CActorManager*									pActorManager;

		CSceneBase*										pParentScene;
		int												m_parentSceneID;

		std::list<CSceneBase*>							pChiledSceneList;
		std::vector<int>								m_chiledSceneIDList;

		int												m_SceneID;

		std::unique_ptr<char[]>							m_SceneName;

	public:
		CSceneBase();
		CSceneBase(const char* sceneName);
		virtual ~CSceneBase();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Draw() override;

		virtual bool Save(std::ofstream& ofs) override;
		virtual bool Load(std::ifstream& ifs) override;

		void SetManager(CShaderManager* pShaderManager, CActorManager* pActorManager);

		void ConnectParentScene(CSceneBase* pParentScene);

		CSceneBase* GetParentScene() const { return pParentScene; }
		int GetParentSceneID() const { return m_parentSceneID; }

		void AddChiledSceneList(CSceneBase* pChiledScene);

		std::list<CSceneBase*> GetChildSceneList() const { return pChiledSceneList; }
		std::vector<int> GetChiledSceneIDList() const { return m_chiledSceneIDList; }
	};

} // namespace

#endif
