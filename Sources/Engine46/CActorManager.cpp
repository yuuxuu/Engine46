/**
 * @file CActorManager.cpp
 * @brief
 * @author 木村優
 * @date 2019/01/02
 */

#include "CActorManager.h"
#include "CSprite.h"
#include "CCamera.h"
#include "CDirectionalLight.h"
#include "CPointLight.h"
#include "CSpotLight.h"
#include "CParticleEmitter.h"

#include "CMesh.h"
#include "CMaterial.h"

#include "CRenderer.h"

namespace Engine46 {

    constexpr const char* g_ActorListFileName = "ActorListData.bin";

    // コンストラクタ
    CActorManager::CActorManager(CRendererBase* pRenderer) :
        pRenderer(pRenderer)
    {}

    // デストラクタ
    CActorManager::~CActorManager()
    {}

    // オブジェクト作成
    CActorBase* CActorManager::CreateActor(ActorType actorType) {
        std::unique_ptr<CActorBase> actor;
        RECT rect;
        std::string actorName;
        CMeshBase* pMesh = nullptr;

        switch (actorType) {
        case ActorType::Root:
            actorName = "Root_" + std::to_string(m_classCount.rootCount++);

            actor = std::make_unique<CActorBase>((UINT)actorType, actorName, Transform());
            break;
        case ActorType::Camera:
            rect = pRenderer->GetWindowRect();
            actorName = "Camera_" + std::to_string(m_classCount.cameraCount++);

            actor = std::make_unique<CCamera>(actorName, rect.w, rect.h);
            break;
        case ActorType::Sprite:
            actorName = "Sprite_" + std::to_string(m_classCount.spriteCount++);

            actor = std::make_unique<CSprite>(actorName);
            break;
        case ActorType::Box:
            actorName = "Box_" + std::to_string(m_classCount.boxCount++);

            actor = std::make_unique<CActorBase>((UINT)actorType, actorName, Transform());
            break;
        case ActorType::Character:
            actorName = "Character_" + std::to_string(m_classCount.charctorCount++);

            actor = std::make_unique<CActorBase>((UINT)actorType, actorName, Transform());
            break;
        case ActorType::ParticleEmitter:
            actorName = "ParticleEmitter_" + std::to_string(m_classCount.particeleEmitterCount++);

            actor = std::make_unique<CParticleEmitter>(actorName);
            break;
        case ActorType::SkyDome:
            actorName = "SkyDome_" + std::to_string(0);

            actor = std::make_unique<CActorBase>((UINT)actorType, actorName, Transform());
            break;
        case ActorType::Light:
            return nullptr;
        }

        std::unique_ptr<CConstantBufferBase> worldConstantBuffer;
        pRenderer->CreateConstantBuffer(worldConstantBuffer, sizeof(worldCB));
        actor->SetWorldConstantBuffer(worldConstantBuffer);

        actor->SetActorID(m_classCount.allCount++);

        //actor->CActorBase::Initialize();

        CActorBase* pActor = actor.get();

        AddActorFromMap(actorName, actor);

        return pActor;
    }

    // ライト作成
    CLight* CActorManager::CreateLight(LightType lightType) {
        std::unique_ptr<CLight> light;
        std::string lightName;

        switch (lightType)
        {
        case LightType::Directional:
            lightName = "DirectionalLight_" + std::to_string(m_classCount.lightCount);

            light = std::make_unique<CDirectionalLight>(lightName);
            break;
        case LightType::Point:
            lightName = "PointLight_" + std::to_string(m_classCount.lightCount);

            light = std::make_unique<CPointLight>(lightName);
            break;
        case LightType::Spot:
            lightName = "SpotLight_" + std::to_string(m_classCount.lightCount);

            light = std::make_unique<CSpotLight>(lightName);
            break;
        }

        std::unique_ptr<CConstantBufferBase> worldConstantBuffer;
        pRenderer->CreateConstantBuffer(worldConstantBuffer, sizeof(worldCB));
        light->SetWorldConstantBuffer(worldConstantBuffer);

        light->SetLightType(lightType);

        light->SetMesh(lightName);

        CMeshBase* pMesh = light->GetMesh();
        if (pMesh) {
            pMesh->SetMaterial(lightName);

            pMesh->CreateSpriteMesh();

            light->CreateOBB();

            CMaterialBase* pMaterial = pMesh->GetMaterial();
            if (pMaterial) {
                pMaterial->SetTexture("particle.png");
            }
        }

        light->SetShaderPackage("CPUParticle.hlsl");

        light->SetActorID(m_classCount.allCount++);
        light->SetLightID(m_classCount.lightCount++);

        light->SetBillBoardEnabled(true);

        light->CActorBase::Initialize();

        CLight* pLight = light.get();

        AddLightFromMap(lightName, light);

        return pLight;
    }

    // オブジェクトをマップへ追加
    void CActorManager::AddActorFromMap(const std::string& name, std::unique_ptr<CActorBase>& pActor) {

        if (!GetActorFromMap(name)) {
            m_pMapActor[name] = std::move(pActor);
            return;
        }
    }

    // オブジェクト取得
    CActorBase* CActorManager::GetActorFromMap(const std::string& name) {
        auto itr = m_pMapActor.find(name);

        if (itr != m_pMapActor.end()) {
            return itr->second.get();
        }

        return nullptr;
    }

    // ライトをマップへ追加
    void CActorManager::AddLightFromMap(const std::string& name, std::unique_ptr<CLight>& pLight) {

        if (!GetLightFromMap(name)) {
            m_pMapLight[name] = std::move(pLight);
            return;
        }
    }

    // ライト取得
    CActorBase* CActorManager::GetLightFromMap(const std::string& name) {
        auto itr = m_pMapLight.find(name);

        if (itr != m_pMapLight.end()) {
            return itr->second.get();
        }

        return nullptr;
    }

    // オブジェクトを保存
    bool CActorManager::SaveActor() {

        std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary;

        std::ofstream ofs;
        ofs.open(g_ActorListFileName, mode);

        if (!ofs.is_open()) return false;

        for (const auto& actor : m_pMapActor) {
            actor.second->Save(ofs);
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

            int classType = -1;
            ifs.read((char*)&classType, sizeof(int));

            if (ifs.eof()) break;

            CActorBase* pActor = this->CreateActor((ActorType)classType);

            pActor->Load(ifs);
        }

        std::cout << g_ActorListFileName << "を読み込みしました。" << std::endl;

        this->ConnectActor();

        return true;
    }

    // オブジェクト同士の接続
    void CActorManager::ConnectActor() {
        for (const auto& actor : m_pMapActor) {
            for (const auto& connectActor : m_pMapActor) {
                
                if (actor == connectActor) continue;

                int parentId = actor.second->GetParentActorID();
                if (parentId > -1) {
                    if (parentId == connectActor.second->GetActorID()) {
                        actor.second->ConnectParentActor(connectActor.second.get());
                    }
                }

                for (const auto childId : actor.second->GetChildActorIDList()) {
                    if (childId == connectActor.second->GetActorID()) {
                        actor.second->AddChiledActorList(connectActor.second.get());
                    }
                }
            }
        }
    }

} // namespace