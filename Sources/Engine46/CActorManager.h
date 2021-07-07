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
	class CActorBase;
	class CRendererBase;

	struct classCount {
		int allCount;
		int rootCount;
		int cameraCount;
		int spriteCount;
		int lightCount;

		classCount() :
			allCount(0),
			rootCount(0),
			cameraCount(0),
			spriteCount(0),
			lightCount(0)
		{}
	};

	class CActorManager {
	private:
		std::vector<std::unique_ptr<CActorBase>>	m_pVecActor;
		
		CActorBase*									pRootActor;

		CRendererBase*								pRenderer;

		classCount									classCount;

	public:
		explicit CActorManager(CRendererBase* pRenderer);
		~CActorManager();

		CActorBase* CreateActor(int classID);

		void AddActorToVec(std::unique_ptr<CActorBase>& pActor) { m_pVecActor.emplace_back(std::move(pActor)); }

		CActorBase* GetActorFromActorName(const char* name);

		bool SaveActor();
		bool LoadActor();

		CActorBase* GetRootActor() const { return pRootActor; }

	private:
		void ConnectActor();

	};
} // namespace

#endif