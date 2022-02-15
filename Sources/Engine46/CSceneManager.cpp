/**
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
    {}

    // デストラクタ
    CSceneManager::~CSceneManager()
    {}

    // シーン作成
    CSceneBase* CSceneManager::CreateScene(const std::string& sceneName) {
        std::unique_ptr<CSceneBase> scene;

        if (!sceneName.empty()) {
            scene = std::make_unique<CSceneBase>(sceneName);
        }
        else {
            scene = std::make_unique<CSceneBase>();
        }

        scene->SetSceneID((UINT)m_pVecScene.size());

        scene->Initialize();

        CSceneBase* pScene = scene.get();

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

            CSceneBase* pScene = CreateScene();

            pScene->Load(ifs);
        }

        std::cout << g_sceneListFileName << "を読み込みしました。" << std::endl;

        return true;
    }

} // namespace