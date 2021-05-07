/**
 * @file CActorManager.h
 * @brief
 * @author 木村優
 * @date 2019/01/02
 */

#pragma once

#ifndef _CACTOR_MANAGER_H_
#define _CACTOR_MANAGER_H_

#include "CActor.h"

namespace Engine46 {
	
	// 前方宣言
	class CDX11Renderer;

	class CActorManager {
	private:
		std::vector<std::unique_ptr<CActorBase>>	m_pVecActor;
		CActorBase*									pRootActor;

		CDX11Renderer*								pDX11Renderer;

		void ConnectActor();

	public:
		explicit CActorManager(CDX11Renderer* pRenderer);
		~CActorManager();

		bool Initialize();

		CActorBase* CreateActor(int classID);

		void CreateMesh(CActorBase* pActor);

		void CreateMaterial(CActorBase* pActor);

		void CreateConstantBuffer(CActorBase* pActor);

		bool SaveActor();
		bool LoadActor();

		void AddActorToActorVec(std::unique_ptr<CActorBase>& pActor) { m_pVecActor.emplace_back(move(pActor)); };

		CActorBase* GetRootActor() const { return pRootActor; }
	};
} // namespace

#endif
