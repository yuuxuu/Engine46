/**
 * @file CActorManager.h
 * @brief
 * @author 木村優
 * @date 2019/01/02
 */

#pragma once

#ifndef _CACTOR_MANAGER_H_
#define _CACTOR_MANAGER_H_

namespace Engine46 {
	
	// 前方宣言
	class CRendererBase;
	class CActorBase;
	class CMeshBase;
	class CMaterialBase;
	class CConstantBufferBase;
	class CShaderPackage;
	class CInput;

	class CActorManager {
	private:
		std::vector<std::unique_ptr<CActorBase>>	m_pVecActor;
		CActorBase*									pRootActor;

		CRendererBase*								pRenderer;

	public:
		explicit CActorManager(CRendererBase* pRenderer);
		~CActorManager();

		bool Initialize();

		CActorBase* CreateActor(int classID);

		void SetMesh(CActorBase* pActor, CMeshBase* pMesh);

		void SetMaterial(CActorBase* pActor, CMaterialBase* pMaterial);

		void SetShaderPackage(CActorBase* pActor, CShaderPackage* pShaderPackage);
		
		void SetInput(CActorBase* pActor, CInput* pInput);

		bool SaveActor();
		bool LoadActor();

		void AddActorToVec(std::unique_ptr<CActorBase>& pActor) { m_pVecActor.emplace_back(move(pActor)); };

		CActorBase* GetRootActor() const { return pRootActor; }

	private:
		void ConnectActor();

	};
} // namespace

#endif
