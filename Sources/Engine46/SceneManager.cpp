/**
 * @file CSceneManager.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/01
 */

#include "SceneManager.h"

namespace Engine46 {
	constexpr const char* g_sceneListFileName = "SceneListData.bin";

	// コンストラクタ
	SceneManager::SceneManager() 
	{}

	// デストラクタ
	SceneManager::~SceneManager()
	{}

	// シーン作成
	CSceneBase* SceneManager::CreateScene(SceneType sceneType) {
		std::unique_ptr<CSceneBase> scene = std::make_unique<CSceneBase>(sceneType);

		CSceneBase* pScene = scene.get();

		AddSceneToSceneList(scene);

		return pScene;
	}

	// シーン同士を接続
	void SceneManager::ConnectScene() {
		for (auto& scene : m_pSceneList) {
			int id = scene->GetParentSceneID();
			if (id > -1) {
				scene->ConnectParentScene(m_pSceneList[id].get());
			}

			for (auto id : scene->GetChiledSceneIDList()) {
				scene->AddChiledSceneList(m_pSceneList[id].get());
			}
		}
	}

	// シーンリスト保存
	bool SceneManager::SaveScene() {
		std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary;

		std::ofstream ofs;
		ofs.open(g_sceneListFileName, mode);

		if (!ofs.is_open()) return false;

		for (const auto& scene : m_pSceneList) {
			scene->Save(ofs);
		}

		return true;
	}

	// シーンリスト読み込み
	bool SceneManager::LoadScene() {
		std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary;

		std::ifstream ifs;
		ifs.open(g_sceneListFileName, mode);

		if (!ifs.is_open()) return false;

		while (true) {

			SceneType type = SceneType::NONE;
			ifs.read((char*)&type, sizeof(int));

			if (ifs.eof()) break;

			CSceneBase* pScene = CreateScene(type);

			pScene->Load(ifs);
		}

		std::cout << g_sceneListFileName << "を読み込みしました。" << std::endl;

		this->ConnectScene();

		return true;
	}

} // namespace