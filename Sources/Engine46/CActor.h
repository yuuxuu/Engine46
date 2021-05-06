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
#include "IObject.h"

namespace Engine46 {
	
	// 前方宣言
	class CDataRecordBase;
	class CMeshBase;

	class CActorBase : public IObject {
	protected:
		std::vector<std::unique_ptr<CDataRecordBase>>	vecDataRecords;

		UINT											m_ClassID;

		int												m_ActorID;

		std::unique_ptr<char[]>							m_ActorName;

		Transform										m_Transform;
		
		std::unique_ptr<CMeshBase>						m_pMesh;

		CActorBase*										pParentActor;
		int												m_parentActorID;

		std::list<CActorBase*>							pChiledActorList;
		std::vector<int>								m_chiledActorIDList;

	public:
		CActorBase();
		CActorBase(const UINT id, const char* name, const Transform transform);
		virtual ~CActorBase();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Draw() override;

		virtual bool Save(std::ofstream& ofs) override;
		virtual bool Load(std::ifstream& ifs) override;

		void ConnectParentActor(CActorBase* pParentActor);

		CActorBase* GetParentActor() const { return pParentActor; }
		int GetParentActorID() const { return m_parentActorID; }

		void AddChiledActorList(CActorBase* pChiledObject);

		std::list<CActorBase*> GetChiledActorList() const { return pChiledActorList; }
		std::vector<int> GetChiledActorIDList() const { return m_chiledActorIDList; }
	};
} // namespace

#endif