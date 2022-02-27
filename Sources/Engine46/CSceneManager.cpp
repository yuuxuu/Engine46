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

    // コンストラクタ
    CSceneManager::CSceneManager()
    {}

    // デストラクタ
    CSceneManager::~CSceneManager()
    {}

    // シーン作成
    CSceneBase* CSceneManager::CreateScene(const std::string& sceneName) {
        CSceneBase* pScene = GetSceneFromMap(sceneName);
        
        if (pScene) return pScene;

        int sceneNo = static_cast<UINT>(m_pMapScene.size());
        std::string name = sceneName;
        if (name.empty()) {
            name = "Scene_" + std::to_string(sceneNo);
        }

        std::unique_ptr<CSceneBase> scene;
        scene = std::make_unique<CSceneBase>(name);

        pScene = scene.get();

        scene->SetSceneID(sceneNo);

        AddSceneToMap(sceneName, scene);

        return pScene;
    }

    // シーンをマップへ追加
    void CSceneManager::AddSceneToMap(const std::string& name, std::unique_ptr<CSceneBase>& pScene) {
        
        if (!GetSceneFromMap(name)) {
            m_pMapScene[name] = std::move(pScene);
        }
    }

    // シーンを取得
    CSceneBase* CSceneManager::GetSceneFromMap(const std::string& name) {
        auto itr = m_pMapScene.find(name);

        if (itr != m_pMapScene.end()) {
            return itr->second.get();
        }

        return nullptr;
    }

} // namespace