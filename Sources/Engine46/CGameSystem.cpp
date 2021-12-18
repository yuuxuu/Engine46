/**
 * @file CGameSystem.h
 * @brief
 * @author 木村優
 * @date 2018/12/19
 */

#include "CGameSystem.h"
#include "CRendererSystem.h"
#include "CWinow.h"
#include "CInput.h"
#include "CScene.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CModelMesh.h"

#include "CActor.h"
#include "CLight.h"
#include "CParticleEmitter.h"

#include "CSceneManager.h"
#include "CActorManager.h"
#include "CMeshManager.h"
#include "CMaterialManager.h"
#include "CTextureManager.h"
#include "CShaderManager.h"

namespace Engine46 {

    // コンストラクタ
    CGameSystem::CGameSystem() :
        m_hGame(nullptr),
        m_isInitialize(false)

    {}

    // デストラクタ
    CGameSystem::~CGameSystem() {
        Finalize();
    }

    // 初期化
    bool CGameSystem::Initialize(CRendererBase* pRenderer, HWND hwnd) {

        if (m_isInitialize) return true;

        // 乱数生成
        srand((unsigned)time(NULL));
        // ロケール設定
        setlocale(LC_CTYPE, "");
        // タイマの分解能力を１ｍｓにする
        timeBeginPeriod(1);

        m_pActorManager = std::make_unique<CActorManager>(pRenderer);

        m_pShaderManager = std::make_unique<CShaderManager>(pRenderer);

        m_pTextureManager = std::make_unique<CTextureManager>(pRenderer);

        m_pMeshManager = std::make_unique<CMeshManager>(pRenderer);

        m_pMaterialManager = std::make_unique<CMaterialManager>(pRenderer);

        m_pSceneManager = std::make_unique<CSceneManager>();

        HINSTANCE hInstance = GetModuleHandle(NULL);

        m_pInput = std::make_unique<CInput>(hwnd);
        if (!m_pInput->Initialize(hInstance)) return false;

        // レンダーシステムにシーンを設定
        CSceneBase* pScene = m_pSceneManager->CreateScene();
        CRendererSystem::GetRendererSystem().SetRenderScene(pScene);

        {
            CActorBase* pRoot = m_pActorManager->CreateActor(ActorType::Root);
            pScene->SetRootActor(pRoot);

            CActorBase* pCamera = m_pActorManager->CreateActor(ActorType::Camera);
            pCamera->SetInput(m_pInput.get());
            pScene->AddActorToScene(pCamera);

            CMeshBase* pMesh = nullptr;
            CModelMesh* pModelMesh = nullptr;

            CActorBase* pSkyDome = m_pActorManager->CreateActor(ActorType::SkyDome);
            pSkyDome->SetModelMesh("SM_SkySphere.FBX");

            pModelMesh = pSkyDome->GetModelMesh();
            if (pModelMesh) {
                std::vector<CMeshBase*> pVecMesh = pModelMesh->GetVecMesh();
                for (const auto& pMesh : pVecMesh) {
                    CMaterialBase* pMaterial = pMesh->GetMaterial();
                    if (!pMaterial) {
                        pMesh->SetMaterial("skydome");

                        pMaterial = pMesh->GetMaterial();
                    }
                    pMaterial->SetTexture("Road_to_MonumentValley_8k.jpg");
                }
            }
            pSkyDome->SetShaderPackage("SkyDome.hlsl");
            pScene->AddActorToScene(pSkyDome);

            /*CActorBase* pBox = m_pActorManager->CreateActor(ActorType::Box);
            pBox->SetMesh("BoxMesh");

            pMesh = pBox->GetMesh();
            if (pMesh) {
                pMesh->SetMaterial("BoxMaterial");

                pMesh->CreateBoxMesh();

                pBox->CreateOBB();

                CMaterialBase* pMaterial = pMesh->GetMaterial();
                if (pMaterial) {
                    pMaterial->SetTexture("E3g6p9QUYAMTSbT.jpg");
                }
            }
            pBox->SetShaderPackage("Model.hlsl");
            pScene->AddActorToScene(pBox);*/

            /*CActorBase* pSphere = m_pActorManager->CreateActor(ActorType::Character);
            pSphere->SetScale(VECTOR3(0.1f, 0.1f, 0.1f));
            pSphere->SetModelMesh("SM_SkySphere.FBX");

            pSphere->SetShaderPackage("Model.hlsl");
            pScene->AddActorToScene(pSphere);*/

            CActorBase* pCharacter = m_pActorManager->CreateActor(ActorType::Character);
            pCharacter->SetModelMesh("star-wars-arc-170-pbr_.fbx");

            pModelMesh = pCharacter->GetModelMesh();
            if (pModelMesh) {
                pCharacter->CreateOBB();
            }
            pCharacter->SetShaderPackage("Model.hlsl");
            pScene->AddActorToScene(pCharacter);

            CLight* pDirectionalLight = m_pActorManager->CreateLight(LightType::Directional);
            pDirectionalLight->SetPos(VECTOR3(0.0f, 0.0f, 1000.0f));
            pScene->AddActorToScene(pDirectionalLight);

            /*CLight* pPointLight = m_pActorManager->CreateLight(LightType::Point);
            pPointLight->SetPos(VECTOR3(0.0f, 0.0f, 10.0f));
            pMesh = pPointLight->GetMesh();
            if (pMesh) {
                CMaterialBase* pMaterial = pMesh->GetMaterial();
                if (pMaterial) {
                    pMaterial->SetDiffuse(VECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
                }
            }
            pPointLight->SetLightDiffuse(VECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
            pScene->AddActorToScene(pPointLight);

            pPointLight = m_pActorManager->CreateLight(LightType::Point);
            pPointLight->SetPos(VECTOR3(5.0f, 0.0f, 10.0f));
            pMesh = pPointLight->GetMesh();
            if (pMesh) {
                CMaterialBase* pMaterial = pMesh->GetMaterial();
                if (pMaterial) {
                    pMaterial->SetDiffuse(VECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
                }
            }
            pPointLight->SetLightDiffuse(VECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
            pScene->AddActorToScene(pPointLight);

            pPointLight = m_pActorManager->CreateLight(LightType::Point);
            pPointLight->SetPos(VECTOR3(-5.0f, 0.0f, 10.0f));
            pMesh = pPointLight->GetMesh();
            if (pMesh) {
                CMaterialBase* pMaterial = pMesh->GetMaterial();
                if (pMaterial) {
                    pMaterial->SetDiffuse(VECTOR4(0.0f, 0.0f, 1.0f, 1.0f));
                }
            }
            pPointLight->SetLightDiffuse(VECTOR4(0.0f, 0.0f, 1.0f, 1.0f));
            pScene->AddActorToScene(pPointLight);*/

            CActorBase* pActor = m_pActorManager->CreateActor(ActorType::ParticleEmitter);
            CParticleEmitter* pParticleEmitter = dynamic_cast<CParticleEmitter*>(pActor);

            UINT numParticle = DEFAULT_MAX_PARTICLE;
            if (pParticleEmitter) {
                pParticleEmitter->Initialize(numParticle);

                std::random_device rd;
                std::mt19937 mt(rd());

                std::uniform_int_distribution<> rand_degree(0, 360);

                std::uniform_real_distribution<float> rand_pos(-300.0f, 300.0f);
                std::uniform_real_distribution<float> rand_color(0.0f, 1.0f);
                std::uniform_real_distribution<float> rand_velocity(-0.1f, 0.1f);
                std::uniform_real_distribution<float> rand_speed(5.0f, 20.0f);

                std::vector<Particle> vecParticle(numParticle);
                for (auto& particle : vecParticle) {
                    particle.pos = VECTOR3(rand_pos(mt), rand_pos(mt), rand_pos(mt));
                    particle.color = VECTOR4(rand_color(mt), rand_color(mt), rand_color(mt), 1.0f);
                    particle.uv = VECTOR2(static_cast<float>(rand_degree(mt)), rand_speed(mt));
                    particle.normal = VECTOR3(rand_velocity(mt), rand_velocity(mt), rand_velocity(mt));

                    particle.initPos = particle.pos;
                    particle.velocity = particle.normal;
                }
                pParticleEmitter->Update(vecParticle);

                pScene->AddActorToScene(pParticleEmitter);
            }
        }

        // イベントハンドル生成
        m_hGame = CreateEvent(NULL, false, false, NULL);
        if (!m_hGame) {
            MessageBox(NULL, "CGamesSystem::CreateEventエラー", "MessageBox", MB_OK);
            return false;
        }
        // ゲームメインスレッド生成
        m_gameSystemThread = std::thread(&CGameSystem::Loop, this);
        if (!m_gameSystemThread.joinable()) {
            std::cout << "ゲームメインスレッド生成:失敗" << std::endl;
            return false;
        }

        m_isInitialize = true;

        return true;
    }

    // 終了
    void CGameSystem::Finalize() {
        // レンダラースレッドを先に終了
        CRendererSystem::GetRendererSystem().Finalize();

        if (m_hGame) {
            CloseHandle(m_hGame);
            m_hGame = 0;
        }
        // ゲームメインスレッドの終了待ち
        if (m_gameSystemThread.joinable()) {
            m_gameSystemThread.join();
        }
        // タイマの分解能力を元に戻す
        timeEndPeriod(1);
    }

    // ループ
    void CGameSystem::Loop() {
        DWORD sts;
        DWORD ms = 1000 / 60; // 1000ms/60fps = 0.16ms

        while (1) {
            sts = WaitForSingleObject(m_hGame, ms);
            if (sts == WAIT_FAILED) {
                break;
            }

            this->Update();
        }
    }

    // 更新
    void CGameSystem::Update() {
        CSceneBase* pScene = CRendererSystem::GetRendererSystem().GetRenderScene();

        if (pScene) {
            pScene->Update();
        }
    }

} // namespace