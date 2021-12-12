/**
 * @file CScene.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CScene.h"
#include "CDataRecord.h"
#include "CActor.h"
#include "CCamera.h"
#include "CLight.h"
#include "CSprite.h"
#include "CParticleEmitter.h"
#include "COBB.h"

namespace Engine46 {

    // コンストラクタ
    CSceneBase::CSceneBase() :
        m_SceneID(0),
        m_SceneName("Scene_" + std::to_string(m_SceneID))
    {
        m_SceneName.resize(m_SceneName.size());
    }

    // コンストラクタ
    CSceneBase::CSceneBase(const char* sceneName) :
        m_SceneID(0),
        m_SceneName(sceneName)
    {
        m_SceneName.resize(m_SceneName.size());
    }

    // デストラクタ
    CSceneBase::~CSceneBase()
    {}

    //初期化
    void CSceneBase::Initialize() {

        vecDataRecords.clear();

        vecDataRecords.emplace_back(CDataRecordBase(offsetof(CSceneBase, m_SceneID), sizeof(m_SceneID)));
        vecDataRecords.emplace_back(CStrDataRecord(offsetof(CSceneBase, m_SceneName), m_SceneName));
    }

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

    // シーン出力
    bool CSceneBase::Save(std::ofstream& ofs) {

        for (auto& records : vecDataRecords) {
            records.WriteData(ofs, (char*)this);
        }

        return true;
    }

    // シーン読み込み
    bool CSceneBase::Load(std::ifstream& ifs) {

        for (auto& records : vecDataRecords) {
            if (&records == &vecDataRecords[0]) continue;

            records.ReadData(ifs, (char*)this);
        }

        return true;
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
            return this->GetActorRecursiveInName(pRootActor, actorName);
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

            for (const auto& pActor : pRootActor->GetChildActorList()) {
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
            if (pRootActor->GetClassID() == (int)ActorType::SkyDome) {
                return pRootActor;
            }
            CActorBase* pSkyDome = this->GetActorRecursiveInActor(pRootActor, (int)ActorType::SkyDome);
            return pSkyDome;
        }

        return nullptr;
    }

    // シーン内のカメラを取得
    CCamera* CSceneBase::GetCameraFromScene() {
        if (pRootActor) {
            if (pRootActor->GetClassID() == (int)ActorType::Camera) {
                return dynamic_cast<CCamera*>(pRootActor);
            }
            CActorBase* pCamera = this->GetActorRecursiveInActor(pRootActor, (int)ActorType::Camera);
            return dynamic_cast<CCamera*>(pCamera);
        }

        return nullptr;
    }

    // シーン内のライトを取得
    CLight* CSceneBase::GetLightFromScene() {
        if (pRootActor) {
            if (pRootActor->GetClassID() == (int)ActorType::Light) {
                return dynamic_cast<CLight*>(pRootActor);
            }
            CActorBase* pLight = this->GetActorRecursiveInActor(pRootActor, (int)ActorType::Light);
            return dynamic_cast<CLight*>(pLight);
        }

        return nullptr;
    }

    // シーン内アクターを全て取得
    std::vector<CActorBase*> CSceneBase::GetActorsFromScene() {
        std::vector<CActorBase*> vecActors;

        if (pRootActor) {
            std::vector<CActorBase*> pActors;
            this->GetActorsRecursiveInActor(pActors, pRootActor, (int)ActorType::Sprite);
            this->GetActorsRecursiveInActor(pActors, pRootActor, (int)ActorType::Box);
            this->GetActorsRecursiveInActor(pActors, pRootActor, (int)ActorType::Character);

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
            this->GetActorsRecursiveInActor(pActors, pRootActor, (int)ActorType::Camera);

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
            this->GetActorsRecursiveInActor(pActors, pRootActor, (int)ActorType::Light);

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
            this->GetActorsRecursiveInActor(pActors, pRootActor, (int)ActorType::ParticleEmitter);

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
                    if (pChild->GetVisible()) {
                        pActors.emplace_back(pChild);
                    }
                }

                this->GetActorsRecursiveInActor(pActors, pChild, actorType);
            }
        }
    }

} // namespace