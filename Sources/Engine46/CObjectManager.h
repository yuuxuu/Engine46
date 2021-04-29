/**
 * @file CObjectManager.h
 * @brief
 * @author –Ø‘º—D
 * @date 2019/01/02
 */

#ifndef _COBJECT_MANAGER_H_
#define _COBJECT_MANAGER_H_

#pragma once

#include "CObject.h"

namespace Engine46 {
	
	class CObjectManager {
	private:
		std::vector<std::unique_ptr<CObject>>	m_pObjectList;

		const char*								m_pObjectListFileName;

	public:
		CObjectManager();
		~CObjectManager();

		CObject* CreateObject(int id);

		bool SaveObjectList();
		bool LoadObjectList();

		int GetElementNumberFromPoint(void* p);

		void AddObjectToObjectList(std::unique_ptr<CObject>& pObject) { m_pObjectList.emplace_back(move(pObject)); };
	
	private:
		void ConnectObject();
	};
} // namespace

#endif
