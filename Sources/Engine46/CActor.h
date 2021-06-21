/**
 * @file CActor.h
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#pragma once

#ifndef _CACTOR_H_
#define _CACTOR_H_

#include "IObject.h"
#include "math.h"

namespace Engine46 {

	// 前方宣言
	class CRendererBase;
	class CDataRecordBase;
	class CConstantBufferBase;
	class CMeshBase;
	class CMaterialBase;
	class CTextureBase;
	class CShaderPackage;
	class CInput;

	enum class ClassType {
		Root,
		Camera,
		Sprite,
	};

	class CActorBase : public IObject {
	protected:
		std::vector<CDataRecordBase>			vecDataRecords;

		UINT									m_classID;

		int										m_actorID;

		std::string								m_actorName;

		Transform								m_transform;
		
		std::unique_ptr<CMeshBase>				m_pMesh;

		std::unique_ptr<CMaterialBase>			m_pMaterial;

		std::unique_ptr<CConstantBufferBase>	m_pConstantBuffer;

		CInput*									pInput;

		CActorBase*								pParentActor;
		int										m_parentActorID;

		std::list<CActorBase*>					pChiledActorList;
		std::vector<int>						m_chiledActorIDList;

	public:
		CActorBase();
		CActorBase(const UINT classID, const char* ActorName, const Transform transform);
		virtual ~CActorBase();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Draw() override;

		virtual bool Save(std::ofstream& ofs) override;
		virtual bool Load(std::ifstream& ifs) override;

		virtual void InitializeResource(CRendererBase* pRenderer) {};

		void SetConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer);

		void SetMesh(std::unique_ptr<CMeshBase>& pMesh);

		void SetMaterial(std::unique_ptr<CMaterialBase>& pMaterial);

		void SetTexture(CTextureBase* pTex);
		void SetTexture(const char* textureName);

		void SetShaderPackage(CShaderPackage* pShaderPackage);
		void SetShaderPackage(const char* shaderPackageName);

		void SetInput(CInput* pInput);

		void ConnectParentActor(CActorBase* pParentActor);

		CActorBase* GetParentActor() const { return pParentActor; }
		int GetParentActorID() const { return m_parentActorID; }

		void AddChiledActorList(CActorBase* pChiledObject);

		std::list<CActorBase*> GetChiledActorList() const { return pChiledActorList; }
		std::vector<int> GetChiledActorIDList() const { return m_chiledActorIDList; }

		UINT GetClassID() const { return m_classID; }

		const char* GetActorName() const { return m_actorName.c_str(); }

		Matrix GetWorldMatrix();

		VECTOR3 GetDirectionVector();

		VECTOR3 GetRightVector();
		VECTOR3 GetUpVector();
		VECTOR3 GetForwardVector();
	};

} // namespace

#endif