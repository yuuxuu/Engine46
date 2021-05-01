/**
 * @file CObjectManager.h
 * @brief
 * @author 木村優
 * @date 2019/01/02
 */

#ifndef _CACTOR_MANAGER_H_
#define _CACTOR_MANAGER_H_

#pragma once

#include "CActor.h"

namespace Engine46 {
	
	class CActorManager {
	private:
		std::vector<std::unique_ptr<CActorBase>>	m_pActorList;

		const char*									m_pActorListFileName;

	public:
		CActorManager();
		~CActorManager();

		CActorBase* CreateActor(int id);

		bool SaveActorList();
		bool LoadActorList();

		int GetElementNumberFromPoint(void* p);

		void AddActorToActorList(std::unique_ptr<CActorBase>& pObject) { m_pActorList.emplace_back(move(pObject)); };
	
	private:
		void ConnectActor();
	};
} // namespace

#endif
