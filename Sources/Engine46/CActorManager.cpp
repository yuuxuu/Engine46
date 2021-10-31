﻿/**
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

#include "GraphicsAPI/CRenderer.h"

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
    CActorBase* CActorManager::CreateActor(int actorType) {
        std::unique_ptr<CActorBase> actor;
        RECT rect;
        std::string actorName;

        switch ((ActorType)actorType) {
        case ActorType::Root:
            actorName = "Root_" + std::to_string(m_classCount.rootCount++);

            actor = std::make_unique<CActorBase>(actorType, actorName.c_str(), Transform());
            break;
        case ActorType::Camera:
            rect = pRenderer->GetWindowRect();
            actorName = "Camera_" + std::to_string(m_classCount.cameraCount++);

            actor = std::make_unique<CCamera>(actorName.c_str(), rect.w, rect.h);
            break;
        case ActorType::Sprite:
            actorName = "Sprite_" + std::to_string(m_classCount.spriteCount++);

            actor = std::make_unique<CSprite>(actorName.c_str());
            actor->SetMesh("SpriteMesh_" + std::to_string(m_classCount.spriteCount++));
            actor->SetMaterial("SpriteMaterial_" + std::to_string(m_classCount.spriteCount++));
            actor->InitializeResource(pRenderer);
            break;
        case ActorType::Light:
            return nullptr;
        }

        actor->SetActorID(m_classCount.allCount++);

        actor->CActorBase::Initialize();

        CActorBase* pActor = actor.get();

        this->AddActorToVec(actor);

        return pActor;
    }

    // ライト作成
    CLight* CActorManager::CreateLight(int lightType) {
        std::unique_ptr<CLight> light;
        std::string lightName;

        switch ((LightType)lightType)
        {
        case LightType::Directional:
            lightName = "DirectionalLight_" + std::to_string(m_classCount.lightCount);

            light = std::make_unique<CDirectionalLight>(lightName.c_str());
            break;
        case LightType::Point:
            lightName = "PointLight_" + std::to_string(m_classCount.lightCount);

            light = std::make_unique<CPointLight>(lightName.c_str());
            break;
        case LightType::Spot:
            lightName = "SpotLight_" + std::to_string(m_classCount.lightCount);

            light = std::make_unique<CSpotLight>(lightName.c_str());
            break;
        }

        light->SetLightType((LightType)lightType);
        light->SetActorID(m_classCount.allCount++);
        light->SetLightID(m_classCount.lightCount++);

        light->SetMesh("LightMesh");
        light->SetMaterial("LightMaterial_" + std::to_string(m_classCount.lightCount));
        light->SetTexture("particle.png");
        light->SetShaderPackage("CPUParticle.hlsl");
        light->InitializeResource(pRenderer);

        light->CActorBase::Initialize();

        CLight* pLight = light.get();

        this->AddLightToVec(light);

        return pLight;
    }

    // オブジェクト取得
    CActorBase* CActorManager::GetActorFromActorName(const char* name) {
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

            int classType = -1;
            ifs.read((char*)&classType, sizeof(int));

            if (ifs.eof()) break;

            CActorBase* pActor = this->CreateActor(classType);

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

            for (auto id : actor->GetChildActorIDList()) {
                actor->AddChiledActorList(m_pVecActor[id].get());
            }
        }
    }

} // namespace