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
#include "CActor.h"
#include "CLight.h"
#include "CScene.h"

#include "CSceneManager.h"
#include "CActorManager.h"
#include "CMeshManager.h"
#include "CMaterialManager.h"
#include "CTextureManager.h"
#include "CShaderManager.h"

#include "../GraphicsAPI/CDX11Renderer.h"

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
		CRendererSystem::GetRendererSystem().SetRenderScene(m_pSceneManager->GetRootScene());

		{
			CActorBase* pCamera = m_pActorManager->CreateActor((int)ClassType::Camera);
			pCamera->SetInput(m_pInput.get());

			CActorBase* pSprite = m_pActorManager->CreateActor((int)ClassType::Sprite);
			pSprite->SetMesh("SpriteMesh");
			pSprite->SetMaterial("SpriteMaterial");
			pSprite->SetTexture("E3g6p9QUYAMTSbT.jpg");
			pSprite->SetShaderPackage("Model.hlsl");
			pSprite->InitializeResource(pRenderer);

			CLight* pDirectionalLight = m_pActorManager->CreateLight((int)LightType::Directional);
			pDirectionalLight->InitializeResource(pRenderer);

			CLight* pPointLight = m_pActorManager->CreateLight((int)LightType::Point);
			pPointLight->InitializeResource(pRenderer);
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
		CSceneBase* pRootScene = m_pSceneManager->GetRootScene();

		if (pRootScene) {
			pRootScene->Update();
		}
	}

} // namespace