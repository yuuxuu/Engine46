/**
 * @file CActorManager.cpp
 * @brief
 * @author 木村優
 * @date 2019/01/02
 */

#include "CActorManager.h"
#include "CActor.h"
#include "CRenderer.h"
#include "CSprite.h"
#include "CCamera.h"

namespace Engine46 {

	constexpr const char* g_ActorListFileName = "ActorListData.bin";

	// コンストラクタ
	CActorManager::CActorManager(CRendererBase* pRenderer) :
		pRenderer(pRenderer)
	{
		pRootActor = this->CreateActor((int)ClassType::Root);
	}

	// デストラクタ
	CActorManager::~CActorManager()
	{}

	// オブジェクト作成
	CActorBase* CActorManager::CreateActor(int classID) {
		std::unique_ptr<CActorBase> actor;
		RECT rect;

		switch ((ClassType)classID) {
		case ClassType::Root:
			actor = std::make_unique<CActorBase>(classID, "RootActor", Transform());
			break;
		case ClassType::Camera:
			rect = pRenderer->GetWindowRect();
			actor = std::make_unique<CCamera>(rect.w, rect.h);
			break;
		case ClassType::Sprite:
			actor = std::make_unique<CSprite>();
			break;
		}

		actor->CActorBase::Initialize();

		CActorBase* pActor = actor.get();

		if (classID != (int)ClassType::Root) {
			pRootActor->AddChiledActorList(pActor);
		}

		this->AddActorToVec(actor);

		return pActor;
	}

	// オブジェクト取得
	CActorBase* CActorManager::GetActor(const char* name) {
		for (const auto& actor : m_pVecActor) {
			if (actor->GetActorName() == name) return actor.get();
		}

		return nullptr;
	}

	// オブジェクトを保存
	bool CActorManager::SaveActor() {

		std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary;

		std::ofstream ofs;
		ofs.open(g_ActorListFileName, mode);
		
		if (!ofs.is_open()) return false;

		for (const auto& actor : m_pVecActor) {
			actor->Save(ofs);
		}

		return true;
	}

	// オブジェクトを読み込み
	bool CActorManager::LoadActor() {

		std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary;

		std::ifstream ifs;
		ifs.open(g_ActorListFileName, mode);

		if (!ifs.is_open()) return false;

		while (true) {

			int id = -1;
			ifs.read((char*)&id, sizeof(int));

			if (ifs.eof()) break;

			CActorBase* pActor = this->CreateActor(id);

			pActor->Load(ifs);
		}

		std::cout << g_ActorListFileName << "を読み込みしました。" << std::endl;

		this->ConnectActor();

		return true;
	}

	// オブジェクト同士の接続
	void CActorManager::ConnectActor() {
		for (const auto& actor : m_pVecActor) {
			int id = actor->GetParentActorID();
			if (id > -1) {
				actor->ConnectParentActor(m_pVecActor[id].get());
			}

			for (auto id : actor->GetChiledActorIDList()) {
				actor->AddChiledActorList(m_pVecActor[id].get());
			}
		}
	}

} // namespace