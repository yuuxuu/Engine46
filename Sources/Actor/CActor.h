/**
 * @file CActor.h
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#pragma once

#ifndef _CACTOR_H_
#define _CACTOR_H_

#include "../Engine46/math.h"
#include "../Engine46/IObject.h"
#include "../Engine46/CDataRecord.h"
#include "../Engine46/CMesh.h"
#include "../Engine46/CMaterial.h"
#include "../Engine46/CConstantBuffer.h"
#include "../Engine46/CInput.h"

namespace Engine46 {

	enum class ClassType {
		Root,
		Camera,
		Sprite,
	};

	class CActorBase : public IObject {
	protected:
		std::vector<std::unique_ptr<CDataRecordBase>>	vecDataRecords;

		UINT											m_ClassID;

		int												m_ActorID;

		std::unique_ptr<char[]>							m_ActorName;

		Transform										m_Transform;
		
		std::unique_ptr<CMeshBase>						m_pMesh;

		std::unique_ptr<CMaterialBase>					m_pMaterial;

		std::unique_ptr<CCbBase>						m_pCb;

		CInput*											pInput;

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

		void CreateConstantBuffer(CDX11Renderer* pRenderer);

		void CreateMesh(CDX11Renderer* pRenderer);

		void CreateMaterial(CDX11Renderer* pRenderer);

		void SetTexture(CTextureBase* pTex);

		void SetShaderPackage(CShaderPackage* pShaderPackage);

		void SetInput(CInput* pInput);

		void ConnectParentActor(CActorBase* pParentActor);

		CActorBase* GetParentActor() const { return pParentActor; }
		int GetParentActorID() const { return m_parentActorID; }

		void AddChiledActorList(CActorBase* pChiledObject);

		std::list<CActorBase*> GetChiledActorList() const { return pChiledActorList; }
		std::vector<int> GetChiledActorIDList() const { return m_chiledActorIDList; }

		UINT GetClassID() const { return m_ClassID; }

		Matrix GetWorldMatrix();


		VECTOR3 GetDirectionVector();

		VECTOR3 GetRightVector();
		VECTOR3 GetUpVector();
		VECTOR3 GetForwardVector();
	};

} // namespace

#endif