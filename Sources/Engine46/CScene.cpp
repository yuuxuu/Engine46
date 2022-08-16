/**
 * @file CScene.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CScene.h"
#include "CActor.h"
#include "CCamera.h"
#include "CLight.h"
#include "CParticleEmitter.h"
#include "COBB.h"

#include "CGameSystem.h"

#include "CFileManager.h"
#include "CActorManager.h"

namespace Engine46 {

    // コンストラクタ
    CSceneBase::CSceneBase() :
        m_sceneID(0),
        m_sceneName("Scene_" + std::to_string(m_sceneID))
    {}

    // コンストラクタ
    CSceneBase::CSceneBase(const std::string& sceneName) :
        m_sceneID(0),
        m_sceneName(sceneName)
    {}

    // デストラクタ
    CSceneBase::~CSceneBase()
    {}

    //初期化
    void CSceneBase::Initialize() 
    {}

    // シーン更新
    void CSceneBase::Update() {
        if (pRootActor) {
            pRootActor->Update();
        }
    }

    // シーン描画
    void CSceneBase::Draw() {
        if (pRootActor) {
            pRootActor->Draw();
        }
    }

    // シーンを保存
    void CSceneBase::SaveScene() {

        std::string inputPath = CFileManager::ResourceRootPath() + m_sceneName + ".json";

        std::ofstream ofs(inputPath, std::ios::out);
        if (!ofs.is_open()) return;

        cereal::JSONOutputArchive archive(ofs);

        if (pRootActor) {
            pRootActor->SerializeActor(archive);
        }
    }

    // シーンを読み込み
    void CSceneBase::LoadScene() {

        FileInfo* fileInfo = CFileManager::GetFileManager().GetFileInfoFromMap(m_sceneName + ".json");
        if (!fileInfo) return;

        std::ifstream ifs(fileInfo->filePath, std::ios::in);
        if (!ifs.is_open()) return;

        CActorManager* pActorManager = CGameSystem::GetGameSystem().GetActorManager();
        if (!pActorManager) return;

        cereal::JSONInputArchive archive(ifs);

        while (archive.getNodeName()) {

            std::string actorName = archive.getNodeName();
            archive.startNode();

            UINT actorType;
            archive.loadValue(actorType);

            CActorBase* pActor = pActorManager->CreateActor(ActorType(actorType), actorName);
            pActor->DeserializeActor(archive);

            AddActorToScene(pActor);

            archive.finishNode();
        }
    }

    // シーンにアクターを追加
    void CSceneBase::AddActorToScene(CActorBase* pActor) {

        if (!pActor) return;

        if (!pRootActor) {
            pRootActor = pActor;
            return;
        }

        pRootActor->AddChiledActorList(pActor);
    }

    // シーン内のアクターを名前で取得
    CActorBase* CSceneBase::GetActorFromMap(std::string& actorName) {
        if (pRootActor) {
            if (pRootActor->GetActorName() == actorName) {
                return pRootActor;
            }
            return GetActorRecursiveInName(pRootActor, actorName);
        }

        return nullptr;
    }

    // シーン内のマウスで選択されたアクターを取得
    CActorBase* CSceneBase::GetMouseSelectActorFromScene(VECTOR2& screenSize, VECTOR2& mousePos) {

        CCamera* pCamera = GetCameraFromScene();
        if (pCamera) {
            Matrix viewProjMat = pCamera->GetViewProjectionMatrix();
            Matrix viewPortMat = GetViewPortMatrix(screenSize.x, screenSize.y);

            Matrix matrix;
            matrix.dx_m = viewProjMat.dx_m * viewPortMat.dx_m;

            Matrix invMatrix;
            invMatrix.dx_m = DirectX::XMMatrixInverse(nullptr, matrix.dx_m);
            
            VECTOR3 nearVec = Vec3TransformCoord(VECTOR3(mousePos.x, mousePos.y, 0.0f), invMatrix);
            VECTOR3 farVec = Vec3TransformCoord(VECTOR3(mousePos.x, mousePos.y, 1.0f), invMatrix);

            Ray ray;
            ray.orgRay = nearVec;
            ray.dirRay = farVec - nearVec;

            for (const auto pActor : pRootActor->GetChildActorList()) {
                if (!pActor->GetVisible()) continue;

                COBB* pObb = pActor->GetOBB();
                if (pObb) {
                    pObb->Update(pActor);

                    if (pObb->IsRayHit(ray)) {
                        return pActor;
                    }
                }
            }
        }

        return nullptr;
    }
    
    // シーン内のスカイドームを取得
    CActorBase* CSceneBase::GetSkyDomeFromScene() {
        if (pRootActor) {
            if (pRootActor->GetClassID() == int(ActorType::SkyDome)) {
                if (pRootActor->GetVisible()) {
                    return pRootActor;
                }
                return nullptr;
            }
            CActorBase* pSkyDome = this->GetActorRecursiveInActor(pRootActor, int(ActorType::SkyDome));
            if (!pSkyDome) return nullptr;

            if (pSkyDome->GetVisible()) {
                return pSkyDome;
            }
        }
        return nullptr;
    }

    // シーン内のカメラを取得
    CCamera* CSceneBase::GetCameraFromScene() {
        if (pRootActor) {
            if (pRootActor->GetClassID() == int(ActorType::Camera)) {
                if (pRootActor->GetVisible()) {
                    return dynamic_cast<CCamera*>(pRootActor);
                }
                return nullptr;
            }
            CActorBase* pCamera = this->GetActorRecursiveInActor(pRootActor, int(ActorType::Camera));
            if (!pCamera) return nullptr;

            if (pCamera->GetVisible()) {
                return dynamic_cast<CCamera*>(pCamera);
            }
        }
        return nullptr;
    }

    // シーン内のライトを取得
    CLight* CSceneBase::GetLightFromScene() {
        if (pRootActor) {
            if (pRootActor->GetClassID() == int(ActorType::Light)) {
                if (pRootActor->GetVisible()) {
                    return dynamic_cast<CLight*>(pRootActor);
                }
                return nullptr;
            }
            CActorBase* pLight = this->GetActorRecursiveInActor(pRootActor, int(ActorType::Light));
            if (!pLight) return nullptr;

            if (pLight->GetVisible()) {
                return dynamic_cast<CLight*>(pLight);
            }
        }
        return nullptr;
    }

    // シーン内アクターを全て取得
    std::vector<CActorBase*> CSceneBase::GetActorsFromScene() {
        std::vector<CActorBase*> vecActors;

        if (pRootActor) {
            std::vector<CActorBase*> pActors;
            GetActorsRecursiveInActor(pActors, pRootActor, int(ActorType::Actor));

            if (!pActors.empty()) {
                for (const auto pActor : pActors) {
                    vecActors.emplace_back(pActor);
                }
            }
        }

        return vecActors;
    }

    // シーン内カメラを全て取得
    std::vector<CCamera*> CSceneBase::GetCamerasFromScene() {
        std::vector<CCamera*> vecCameras;

        if (pRootActor) {
            std::vector<CActorBase*> pActors;
            GetActorsRecursiveInActor(pActors, pRootActor, int(ActorType::Camera));

            if (!pActors.empty()) {
                for (const auto pActor : pActors) {
                    vecCameras.emplace_back(dynamic_cast<CCamera*>(pActor));
                }
            }
        }

        return vecCameras;
    }

    // シーン内のライトを全て取得
    std::vector<CLight*> CSceneBase::GetLightsFromScene() {
        std::vector<CLight*> vecLights;

        if (pRootActor) {
            std::vector<CActorBase*> pActors;
            GetActorsRecursiveInActor(pActors, pRootActor, int(ActorType::Light));

            if (!pActors.empty()) {
                for (const auto pActor : pActors) {
                    vecLights.emplace_back(dynamic_cast<CLight*>(pActor));
                }
            }
        }

        return vecLights;
    }

    // シーン内のパーティクルエミッターを全て取得
    std::vector<CParticleEmitter*> CSceneBase::GetParticleEmittersFromScene() {
        std::vector<CParticleEmitter*> vecParticleEmitters;

        if (pRootActor) {
            std::vector<CActorBase*> pActors;
            GetActorsRecursiveInActor(pActors, pRootActor, int(ActorType::ParticleEmitter));

            if (!pActors.empty()) {
                for (const auto pActor : pActors) {
                    vecParticleEmitters.emplace_back(dynamic_cast<CParticleEmitter*>(pActor));
                }
            }
        }

        return vecParticleEmitters;
    }

    // 指定アクター名のアクターを再帰検索して取得
    CActorBase* CSceneBase::GetActorRecursiveInName(CActorBase* pRootActor, std::string& actorName) {
        if (pRootActor) {
            for (const auto pChild : pRootActor->GetChildActorList()) {
                if (pChild->GetActorName() == actorName) {
                    return pChild;
                }

                this->GetActorRecursiveInName(pChild, actorName);
            }
        }

        return nullptr;
    }

    // 指定アクターを再帰検索して取得
    CActorBase* CSceneBase::GetActorRecursiveInActor(CActorBase* pRootActor, int actorType) {
        if (pRootActor) {
            for (const auto pChild : pRootActor->GetChildActorList()) {
                if (pChild->GetClassID() == actorType) {
                    return pChild;
                }

                this->GetActorRecursiveInActor(pChild, actorType);
            }
        }

        return nullptr;
    }

    // 指定アクター名のアクター全てを再帰検索して取得
    void CSceneBase::GetActorsRecursiveInName(std::vector<CActorBase*>& pActors, CActorBase* pRootActor, std::string& actorName) {
        if (pRootActor) {
            for (const auto pChild : pRootActor->GetChildActorList()) {
                if (pChild->GetActorName() == actorName) {
                    if (pChild->GetVisible()) {
                        pActors.emplace_back(pChild);
                    }
                }

                this->GetActorsRecursiveInName(pActors, pChild, actorName);
            }
        }
    }

    // 指定アクター全てを再帰検索して取得
    void CSceneBase::GetActorsRecursiveInActor(std::vector<CActorBase*>& pActors, CActorBase* pRootActor, int actorType) {
        if (pRootActor) {
            for (const auto pChild : pRootActor->GetChildActorList()) {
                if (pChild->GetClassID() == actorType) {
                    pActors.emplace_back(pChild);
                }

                this->GetActorsRecursiveInActor(pActors, pChild, actorType);
            }
        }
    }

} // namespace