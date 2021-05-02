/**
 * @file CActorManager.cpp
 * @brief
 * @author 木村優
 * @date 2019/01/02
 */

#include "CActorManager.h"
#include "CFileSystem.h"

namespace Engine46 {

	constexpr const char* g_ActorListFileName = "ActorListData.bin";

	// コンストラクタ
	CActorManager::CActorManager()
	{}

	// デストラクタ
	CActorManager::~CActorManager()
	{}

	// オブジェクト作成
	CActorBase* CActorManager::CreateActor(int id) {
		std::unique_ptr<CActorBase> actor = std::make_unique<CActorBase>();

		CActorBase* pActor = actor.get();

		AddActorToActorList(actor);

		return pActor;
	}

	// オブジェクトリストを保存
	bool CActorManager::SaveActorList() {

		std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary;

		std::ofstream ofs;
		ofs.open(g_ActorListFileName, mode);

		if (!ofs.is_open()) return false;

		for (const auto& actor : m_pActorList) {
			actor->Save(ofs);
		}

		return true;
	}

	// オブジェクトリストを読み込み
	bool CActorManager::LoadActorList() {

		std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary;

		std::ifstream ifs;
		ifs.open(g_ActorListFileName, mode);

		if (!ifs.is_open()) return false;

		while (true) {

			int id = -1;
			ifs.read((char*)&id, sizeof(int));

			if (ifs.eof()) break;

			CActorBase* pActor = CreateActor(id);

			pActor->Load(ifs);
		}

		std::cout << g_ActorListFileName << "を読み込みしました。" << std::endl;

		this->ConnectActor();

		return true;
	}

	// オブジェクト同士の接続
	void CActorManager::ConnectActor() {
		for (const auto& actor : m_pActorList) {
			int id = actor->GetParentActorID();
			if (id > -1) {
				actor->ConnectParentActor(m_pActorList[id].get());
			}

			for (auto id : actor->GetChiledActorIDList()) {
				actor->AddChiledActorList(m_pActorList[id].get());
			}
		}
	}

} // namespace