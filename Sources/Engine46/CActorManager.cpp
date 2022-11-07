/**
 * @file CActorManager.cpp
 * @brief
 * @author 木村優
 * @date 2019/01/02
 */

#include "CActorManager.h"
#include "CCamera.h"
#include "CDirectionalLight.h"
#include "CPointLight.h"
#include "CSpotLight.h"
#include "CParticleEmitter.h"

#include "CMesh.h"
#include "CMaterial.h"

#include "CRenderer.h"

#include "utility.h"

namespace Engine46 {

    // コンストラクタ
    CActorManager::CActorManager(CRendererBase* pRenderer) :
        pRenderer(pRenderer)
    {}

    // デストラクタ
    CActorManager::~CActorManager()
    {}

    // オブジェクト作成
    CActorBase* CActorManager::CreateActor(ActorType actorType, const std::string& actorName) {
        std::unique_ptr<CActorBase> actor;
        std::string name = actorName;
        RECT rect;

        if (name.empty()) {
            switch (actorType) {
            case ActorType::Root:
                name = "Root_" + std::to_string(m_classCount.rootCount++);
                break;
            case ActorType::Camera:
                name = "Camera_" + std::to_string(m_classCount.cameraCount++);
                break;
            case ActorType::Actor:
                name = "Actor_" + std::to_string(m_classCount.actorCount++);
                break;
            case ActorType::ParticleEmitter:
                name = "ParticleEmitter_" + std::to_string(m_classCount.particeleEmitterCount++);
                break;
            case ActorType::SkyDome:
                name = "SkyDome_" + std::to_string(m_classCount.skyDomeCount++);
                break;
            case ActorType::Light:
                return nullptr;
            }
        }

        if (actorType == ActorType::Camera) {
            rect = pRenderer->GetWindowRect();
            actor = std::make_unique<CCamera>(name, rect.w, rect.h);
        }
        else if (actorType == ActorType::ParticleEmitter) {
            actor = std::make_unique<CParticleEmitter>(name);
        }
        else {
            actor = std::make_unique<CActorBase>(UINT(actorType), name, Transform());
        }

        std::unique_ptr<CConstantBufferBase> worldConstantBuffer;
        pRenderer->CreateConstantBuffer(worldConstantBuffer, sizeof(worldCB));
        actor->SetWorldConstantBuffer(worldConstantBuffer);

        GUID id;
        if (SUCCEEDED(CoCreateGuid(&id)))
        {
            RPC_WSTR wString;
            if (RPC_S_OK == UuidToStringW(&id, &wString))
            {
                std::string str;
                std::wstring outString =(WCHAR*)wString;

                WStringConvertToStrig(outString, str);

                actor->SetActorID(str);

                RpcStringFreeW(&wString);
            }
        }

        CActorBase* pActor = actor.get();

        AddActorFromMap(name, actor);

        return pActor;
    }

    // ライト作成
    CLight* CActorManager::CreateLight(LightType lightType, const std::string& lightName) {
        std::unique_ptr<CLight> light;
        std::string name = lightName;

        if (name.empty()) {
            switch (lightType) {
            case LightType::Directional:
                name = "DirectionalLight_" + std::to_string(m_classCount.lightCount);
                break;
            case LightType::Point:
                name = "PointLight_" + std::to_string(m_classCount.lightCount);
                break;
            case LightType::Spot:
                name = "SpotLight_" + std::to_string(m_classCount.lightCount);
                break;
            }
        }

        if (lightType == LightType::Directional) {
            light = std::make_unique<CDirectionalLight>(name);
        }
        else if (lightType == LightType::Point) {
            light = std::make_unique<CPointLight>(name);
        }
        else if (lightType == LightType::Spot) {
            light = std::make_unique<CSpotLight>(name);
        }

        std::unique_ptr<CConstantBufferBase> worldConstantBuffer;
        pRenderer->CreateConstantBuffer(worldConstantBuffer, sizeof(worldCB));
        light->SetWorldConstantBuffer(worldConstantBuffer);

        light->SetMesh("LightMesh");

        CMeshBase* pMesh = light->GetMesh();
        if (pMesh) {
            pMesh->SetMaterial(name);

            pMesh->CreateSpriteMesh();

            light->CreateOBB();

            CMaterialBase* pMaterial = pMesh->GetMaterial();
            if (pMaterial) {
                pMaterial->SetTexture("particle.png");
            }
        }

        light->SetShaderPackage("CPUParticle.hlsl");

        GUID id;
        if (SUCCEEDED(CoCreateGuid(&id)))
        {
            RPC_WSTR wString;
            if (RPC_S_OK == UuidToStringW(&id, &wString))
            {
                std::string str;
                std::wstring outString = (WCHAR*)wString;

                WStringConvertToStrig(outString, str);

                light->SetActorID(str);

                RpcStringFreeW(&wString);
            }
        }

        light->SetBillBoardEnabled(true);

        CLight* pLight = light.get();

        AddLightFromMap(name, light);

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

    // オブジェクト同士の接続
    void CActorManager::ConnectActor() {
        for (const auto& actor : m_pMapActor) {
            for (const auto& connectActor : m_pMapActor) {
                
                if (actor == connectActor) continue;

                std::string parentIdStr = actor.second->GetParentActorID();
                if (!parentIdStr.empty()) {
                    if (parentIdStr == connectActor.second->GetActorID()) {
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