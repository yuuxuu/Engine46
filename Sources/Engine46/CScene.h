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

namespace Engine46 {

	// 前方宣言
	class CDataRecordBase;
	class CActorBase;
	class CCamera;
	class CLight;

	class CSceneBase {
	protected:
		std::vector<CDataRecordBase>	vecDataRecords;

		CActorBase*						pRootActor;

		UINT							m_SceneID;

		std::string						m_SceneName;

	public:
		CSceneBase();
		explicit CSceneBase(const char* sceneName);
		virtual ~CSceneBase();

		virtual void Initialize();
		virtual void Update();
		virtual void Draw();

		virtual bool Save(std::ofstream& ofs);
		virtual bool Load(std::ifstream& ifs);

		void SetRootActor(CActorBase* pRootActor) { this->pRootActor = pRootActor; }
		CActorBase* GetRootActor() const { return pRootActor; }

		CActorBase* GetActorFromActorName(std::string& actorName);
		
		CCamera* GetCameraFromScene();
		CLight* GetLightFromScene();

		std::vector<CCamera*> GetCamerasFromScene();
		std::vector<CLight*> GetLightsFromScene();

		void SetSceneID(const int id) { m_SceneID = id; }

		void SetSceneName(const std::string& sceneName) { m_SceneName = sceneName; }
		std::string GetSceneName() const { return m_SceneName.c_str(); }

	private:
		CActorBase* GetActorRecursiveInName(CActorBase* pRootActor, std::string& actorName);
		CActorBase* GetActorRecursiveInClass(CActorBase* pRootActor, int classID);

		void GetActorsRecursiveInName(std::vector<CActorBase*>& pActors, CActorBase* pRootActor, std::string& actorName);
		void GetActorsRecursiveInClass(std::vector<CActorBase*>& pActors, CActorBase* pRootActor, int classID);
	};

} // namespace

#endif
