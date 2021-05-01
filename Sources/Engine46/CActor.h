/**
 * @file CActor.h
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#ifndef _CACTOR_H_
#define _CACTOR_H_

#pragma once

#include "math.h"
#include "ObjectInterface.h"

namespace Engine46 {
	
	class CActorBase : public ObjectInterface {
	protected:
		UINT							m_ClassID;

		CActorBase*						pParentObject;
		int								m_parentObjectID;

		std::list<CActorBase*>			pChiledObjectList;
		std::vector<int>				m_chiledObjectIDList;

		int								m_ObjectID;

		std::unique_ptr<char[]>			m_Name;

		Transform						m_Transform;

	public:
		CActorBase();
		CActorBase(UINT id, const char* name, const Transform transform);
		virtual ~CActorBase();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Draw() override;

		virtual bool Save(std::ofstream& ofs) override;
		virtual bool Load(std::ifstream& ifs) override;

		void ConnectParentObject(CActorBase* pParentObject) { this->pParentObject = pParentObject; }
		CActorBase* GetParentObject() const { return pParentObject; }
		int GetParentObjectID() const { return m_parentObjectID; }

		void AddChiledObjectList(CActorBase* pChiledObject) { pChiledObjectList.emplace_back(pChiledObject); }
		std::list<CActorBase*> GetChiledObjectList() const { return pChiledObjectList; }

		std::vector<int> GetChiledObjectIDList() const { return m_chiledObjectIDList; }
	};
} // namespace

#endif