/**
 * @file CGameSystem.h
 * @brief
 * @author 木村優
 * @date 2018/12/19
 */

#include "CGameSystem.h"
#include "CRendererSystem.h"
#include "CThreadPoolSystem.h"

#include "CWinow.h"
#include "CInput.h"
#include "CScene.h"

#include "CMaterial.h"
#include "CMesh.h"
#include "CModelMesh.h"

#include "CActor.h"
#include "CCamera.h"
#include "CLight.h"
#include "CPointLight.h"
#include "CParticleEmitter.h"

#include "CSceneManager.h"
#include "CActorManager.h"
#include "CMeshManager.h"
#include "CMaterialManager.h"
#include "CTextureManager.h"
#include "CShaderManager.h"
#include "CFileManager.h"

namespace Engine46 {

    // コンストラクタ
    CGameSystem::CGameSystem() :
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

        m_hwnd = hwnd;
        m_pInput = std::make_unique<CInput>(m_hwnd);

        HINSTANCE hInstance = GetModuleHandle(NULL);
        if (!m_pInput->Initialize(hInstance)) return false;

        // レンダーシステムにシーンを設定
        CSceneBase* pScene = CSceneManager::GetSceneManager().CreateScene();
        CRendererSystem::GetRendererSystem().SetRenderScene(pScene);
        if (pScene) {
            CCamera* pCamera = pScene->GetCameraFromScene();
            pCamera->SetInput(m_pInput.get());
        }

        if (!CFileManager::GetFileManager().Initialize()) {
            return false;
        }

        {
            /*CActorBase* pRoot = m_pActorManager->CreateActor(ActorType::Root);
            pScene->SetRootActor(pRoot);

            CActorBase* pCamera = m_pActorManager->CreateActor(ActorType::Camera);
            if(CCamera* pCam = dynamic_cast<CCamera*>(pCamera))
            {
                pCam->SetInput(m_pInput.get());
                pScene->AddActorToScene(pCam);
            }

            pScene->SaveScene();*/

            CMeshBase* pMesh = nullptr;
            CModelMesh* pModelMesh = nullptr;

            /*CActorBase* pSkyDome = m_pActorManager->CreateActor(ActorType::SkyDome);
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
            pScene->AddActorToScene(pSkyDome);*/

            /*CActorBase* pPlane = m_pActorManager->CreateActor(ActorType::Sprite);
            pPlane->SetMesh("PlaneMesh");

            pMesh = pPlane->GetMesh();
            if (pMesh) {
                pMesh->SetMaterial("PlaneMaterial");

                pMesh->CreateSpriteMesh();

                pPlane->CreateOBB();

                CMaterialBase* pMaterial = pMesh->GetMaterial();
                if (pMaterial) {
                    pMaterial->SetTexture("floor_tiles_06_diff_4k.jpg");
                }
            }
            pPlane->SetRotation(VECTOR3(DegreeToRadian(-90.0f), 0.0f, 0.0f));
            pPlane->SetScale(VECTOR3(100.0f, 100.0f, 1.0f));
            pPlane->SetShaderPackage("Model.hlsl");
            pScene->AddActorToScene(pPlane);*/

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

            /*CActorBase* pSphere = m_pActorManager->CreateActor(ActorType::Actor);
            pSphere->SetScale(VECTOR3(0.1f, 0.1f, 0.1f));
            pSphere->SetModelMesh("SM_SkySphere.FBX");

            pSphere->SetShaderPackage("Model.hlsl");
            pScene->AddActorToScene(pSphere);*/

            /*CActorBase* pCharacter = m_pActorManager->CreateActor(ActorType::Actor);
            pCharacter->SetModelMesh("sponza.obj");

            pModelMesh = pCharacter->GetModelMesh();
            if (pModelMesh) {
                pCharacter->CreateOBB();
            }
            pCharacter->SetShaderPackage("Model.hlsl");
            pScene->AddActorToScene(pCharacter);*/

            CLight* pDirectionalLight = m_pActorManager->CreateLight(LightType::Directional);
            pDirectionalLight->SetVisible(false);
            pScene->AddActorToScene(pDirectionalLight);

            /*std::random_device rd;
            std::mt19937 mt(rd());

            std::uniform_real_distribution<float> rand_color(0.0f, 1.0f);
            std::uniform_real_distribution<float> rand_radius(50.0f, 100.0f);
            std::uniform_real_distribution<float> rand_posY(0.0f, 1000.0f);

            const int numLight = LIGHT_MAX;
            for (int i = 0; i < numLight; ++i) {
                CLight* pLight = m_pActorManager->CreateLight(LightType::Point);
                pLight->SetVisible(false);

                CPointLight* pPointLight = dynamic_cast<CPointLight*>(pLight);
                if (pPointLight) {
                    pPointLight->SetPos(VECTOR3(0.0f, rand_posY(mt), 0.0f));
                    pPointLight->SetRadius(rand_radius(mt));
                    pPointLight->SetLightDiffuse(VECTOR4(rand_color(mt), rand_color(mt), rand_color(mt), 1.0f));
                }

                pScene->AddActorToScene(pLight);
            }*/

            CActorBase* pActor = m_pActorManager->CreateActor(ActorType::ParticleEmitter);
            CParticleEmitter* pParticleEmitter = dynamic_cast<CParticleEmitter*>(pActor);

            UINT numParticle = DEFAULT_MAX_PARTICLE;
            if (pParticleEmitter) {
                pParticleEmitter->Initialize(numParticle);

                std::random_device rd;
                std::mt19937 mt(rd());

                std::uniform_int_distribution<> rand_degree(0, 360);

                std::uniform_real_distribution<float> rand_pos(-300.0f, 300.0f);
                std::uniform_real_distribution<float> rand_color(1.0f, 2.0f);
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
            if (sts == WAIT_FAILED) break;

            Update();
        }
    }

    // 更新
    void CGameSystem::Update() {
        CRendererBase* pRenderer = CRendererSystem::GetRendererSystem().GetRenderer();
        CSceneBase* pScene = CRendererSystem::GetRendererSystem().GetRenderScene();

        if (!pRenderer || !pScene || !m_pInput) return;

        pScene->Update();
    }

} // namespace