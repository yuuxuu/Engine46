/**
 * @file CActor.h
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#pragma once

#ifndef _CACTOR_H_
#define _CACTOR_H_

#include "math.h"
#include "IObject.h"
#include "CDataRecord.h"
#include "CConstantBuffer.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CTexture.h"
#include "CInput.h"

namespace Engine46 {

	enum class ClassType {
		Root,
		Camera,
		Sprite,
	};

	class CActorBase : public IObject {
	protected:
		std::vector<std::unique_ptr<CDataRecordBase>>	vecDataRecords;

		UINT											m_classID;

		int												m_actorID;

		std::unique_ptr<char[]>							m_actorName;

		Transform										m_transform;
		
		CMeshBase*										m_pMesh;

		CMaterialBase*									m_pMaterial;

		std::unique_ptr<CConstantBufferBase>			m_pConstantBuffer;

		CInput*											pInput;

		CActorBase*										pParentActor;
		int												m_parentActorID;

		std::list<CActorBase*>							pChiledActorList;
		std::vector<int>								m_chiledActorIDList;

	public:
		CActorBase();
		CActorBase(const UINT classID, const char* name, const Transform transform);
		virtual ~CActorBase();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Draw() override;

		virtual bool Save(std::ofstream& ofs) override;
		virtual bool Load(std::ifstream& ifs) override;

		void SetConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer);

		void SetMesh(CMeshBase* pMesh);

		void SetMaterial(CMaterialBase*  pMaterial);

		void SetTexture(CTextureBase* pTex);

		void SetShaderPackage(CShaderPackage* pShaderPackage);

		void SetInput(CInput* pInput);

		void ConnectParentActor(CActorBase* pParentActor);

		CActorBase* GetParentActor() const { return pParentActor; }
		int GetParentActorID() const { return m_parentActorID; }

		void AddChiledActorList(CActorBase* pChiledObject);

		std::list<CActorBase*> GetChiledActorList() const { return pChiledActorList; }
		std::vector<int> GetChiledActorIDList() const { return m_chiledActorIDList; }

		UINT GetClassID() const { return m_classID; }

		Matrix GetWorldMatrix();

		VECTOR3 GetDirectionVector();

		VECTOR3 GetRightVector();
		VECTOR3 GetUpVector();
		VECTOR3 GetForwardVector();
	};

} // namespace

#endif