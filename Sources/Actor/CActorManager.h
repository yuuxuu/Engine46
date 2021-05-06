/**
 * @file CActorManager.h
 * @brief
 * @author 木村優
 * @date 2019/01/02
 */

#ifndef _CACTOR_MANAGER_H_
#define _CACTOR_MANAGER_H_

#pragma once

#include "CActor.h"

namespace Engine46 {
	
	class CDX11Renderer;

	class CActorManager {
	private:
		std::vector<std::unique_ptr<CActorBase>>	m_pActorList;

		CDX11Renderer*								pDX11Renderer;

	public:
		explicit CActorManager(CDX11Renderer* pRenderer);
		~CActorManager();

		CActorBase* CreateActor(int id);

		void CreateMeshForActor(CActorBase* pActor);

		void DrawActor();

		bool SaveActorList();
		bool LoadActorList();

		void AddActorToActorList(std::unique_ptr<CActorBase>& pActor) { m_pActorList.emplace_back(move(pActor)); };
	
	private:
		void ConnectActor();
	};
} // namespace

#endif
