/**
 * @file CGameSystem.h
 * @brief
 * @author 木村優
 * @date 2018/12/19
 */

#include "CGameSystem.h"
#include "CWinow.h"
#include "CInput.h"
#include "CTimer.h"
#include "CActor.h"
#include "CScene.h"

#include "CSceneManager.h"
#include "CActorManager.h"
#include "CMeshManager.h"
#include "CMaterialManager.h"
#include "CTextureManager.h"
#include "CShaderManager.h"

#include "CConstantBuffer.h"

#include "../GraphicsAPI/CDX11Renderer.h"

namespace Engine46 {

	// コンストラクタ
	CGameSystem::CGameSystem() :
		m_hGame(nullptr)
	{}

	// デストラクタ
	CGameSystem::~CGameSystem() {
		Finalize();
	}

	// 初期化
	bool CGameSystem::Initialize(CRendererBase* renderer, HWND hwnd, RECT rect) {
		// 乱数生成
		srand((unsigned)time(NULL));
		// ロケール設定
		setlocale(LC_CTYPE, "");
		// タイマの分解能力を１ｍｓにする
		timeBeginPeriod(1);

		m_pActorManager = std::make_unique<CActorManager>(renderer);

		m_pShaderManager = std::make_unique<CShaderManager>(renderer);

		m_pTextureManager = std::make_unique<CTextureManager>(renderer);

		m_pMeshManager = std::make_unique<CMeshManager>(renderer);

		m_pMaterialManager = std::make_unique<CMaterialManager>(renderer);

		m_pSceneManager = std::make_unique<CSceneManager>();

		HINSTANCE hInstance = GetModuleHandle(NULL);
		
		m_pInput = std::make_unique<CInput>(hwnd);
		if (!m_pInput->Initialize(hInstance)) return false;

		{
			CSceneBase* rootScene = m_pSceneManager->GetRootScene();
			CActorBase* rootActor = m_pActorManager->GetRootActor();
			rootScene->SetRootActor(rootActor);

			CActorBase* pCamera = m_pActorManager->CreateActor((int)ClassType::Camera);
			pCamera->SetInput(m_pInput.get());

			CActorBase* pSprite = m_pActorManager->CreateActor((int)ClassType::Sprite);

			std::unique_ptr<CConstantBufferBase> cb;
			renderer->CreateConstantBuffer(cb);
			pSprite->SetConstantBuffer(cb);

			m_pMeshManager->SetMeshToActor(pSprite);
			m_pMaterialManager->SetMaterialToActor(pSprite);
			m_pShaderManager->SetShaderPackageToActor(pSprite, "Model.hlsl");

			pSprite->Initialize();
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
			if (sts == WAIT_FAILED) {
				break;
			}

			CTimer timer;

			this->Update();
		}
	}

	// 更新
	void CGameSystem::Update() {
		if (m_pInput) {
			m_pInput->UpdateInput();
		}

		CSceneBase* pRootScene = m_pSceneManager->GetRootScene();

		if (pRootScene) {
			pRootScene->Update();
		}
	}

} // namespace