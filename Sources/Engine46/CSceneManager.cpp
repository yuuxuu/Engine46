﻿/**
 * @file CSceneManager.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/01
 */

#include "CSceneManager.h"
#include "CScene.h"
#include "CGameSystem.h"
#include "CActor.h"
#include "CActorManager.h"

namespace Engine46 {

	constexpr const char* g_sceneListFileName = "SceneListData.bin";

	// コンストラクタ
	CSceneManager::CSceneManager()
	{
		pRootScene = this->CreateScene(0);
	}

	// デストラクタ
	CSceneManager::~CSceneManager()
	{}

	// シーン作成
	CSceneBase* CSceneManager::CreateScene(int id) {
		std::unique_ptr<CSceneBase> scene = std::make_unique<CSceneBase>();

		scene->Initialize();

		CSceneBase* pScene = scene.get();

		CActorManager* actorManager = CGameSystem::GetGameSystem().GetActorManager();

		pScene->SetRootActor(actorManager->GetRootActor());

		if (id != 0) {
			pRootScene->AddChiledSceneList(pScene);
		}

		this->AddSceneToVec(scene);

		return pScene;
	}

	// シーン保存
	bool CSceneManager::SaveScene() {
		std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary;

		std::ofstream ofs;
		ofs.open(g_sceneListFileName, mode);

		if (!ofs.is_open()) return false;

		for (const auto& scene : m_pVecScene) {
			scene->Save(ofs);
		}

		return true;
	}

	// シーン読み込み
	bool CSceneManager::LoadScene() {
		std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary;

		std::ifstream ifs;
		ifs.open(g_sceneListFileName, mode);

		if (!ifs.is_open()) return false;

		while (true) {

			if (ifs.eof()) break;

			int id = -1;
			ifs.read((char*)&id, sizeof(int));

			CSceneBase* pScene = CreateScene(id);

			pScene->Load(ifs);
		}

		std::cout << g_sceneListFileName << "を読み込みしました。" << std::endl;

		this->ConnectScene();

		return true;
	}

	// シーン同士の接続
	void CSceneManager::ConnectScene() {
		for (auto& scene : m_pVecScene) {
			int id = scene->GetParentSceneID();
			if (id > -1) {
				scene->ConnectParentScene(m_pVecScene[id].get());
			}

			for (auto id : scene->GetChiledSceneIDList()) {
				scene->AddChiledSceneList(m_pVecScene[id].get());
			}
		}
	}

} // namespace