/**
 * @file CGameSystem.h
 * @brief
 * @author 木村優
 * @date 2018/12/19
 */

#include "CGameSystem.h"
#include "CWinow.h"

#include "../Renderer/CDX11Renderer.h"

#include "../Scene/CSceneManager.h"

namespace Engine46 {

	// コンストラクタ
	CGameSystem::CGameSystem() :
		m_hGame(nullptr),
		m_mainWindow(nullptr),
		m_fps(0),
		m_wfps(0),
		m_oldTime(0),
		m_nowTime(0) 
	{}

	// デストラクタ
	CGameSystem::~CGameSystem() {
		Finalize();
	}

	// 初期化
	bool CGameSystem::Initialize(HINSTANCE hInstance) {
		// 乱数生成
		srand((unsigned)time(NULL));
		// ロケール設定
		setlocale(LC_CTYPE, "");
		// タイマの分解能力を１ｍｓにする
		timeBeginPeriod(1);
		// メインウインドウ作成
		m_mainWindow = std::make_unique<CWindow>();
		// メインウインドウ初期化
		if (!m_mainWindow->Initialize(hInstance, "MainWindow", "Engine46")) {
			std::cout << "ウインドウ初期化:失敗" << std::endl;
			return false;
		}

		HWND hwnd = m_mainWindow->GetHwnd();
		RECT rect = m_mainWindow->GetWindowSize();

		m_pDX11Renderer = std::make_unique<CDX11Renderer>();
		if (!m_pDX11Renderer->Initialize(hwnd, rect.w, rect.h)) return false;

		m_pSceneManager = std::make_unique<CSceneManager>(m_pDX11Renderer.get());
		if (!m_pSceneManager->Initialize()) return false;

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

	// ゲームシステムの終了
	void CGameSystem::Finalize() {
		if (m_hGame) {
			CloseHandle(m_hGame);
			m_hGame = 0;
		}
		// ゲームメインスレッドの終了を待つ
		if (m_gameSystemThread.joinable()) {
			m_gameSystemThread.join();
		}
		// タイマの分解能力を元に戻す
		timeEndPeriod(1);
	}

	// ループ
	void CGameSystem::Loop() {
		DWORD sts;
		while (1) {
			// 1000ms(1秒)待つ
			sts = WaitForSingleObject(m_hGame, 1000);
			if (sts == WAIT_FAILED) {
				break;
			}

			Update();

			Draw();

			MeasFPS();
		}
	}

	// 更新
	void CGameSystem::Update() {
		CSceneBase* scene = m_pSceneManager->GetRootScene();

		if (scene) {
			scene->Update();
		}
	}

	// 描画
	void CGameSystem::Draw() {
		CSceneBase* scene = m_pSceneManager->GetRootScene();

		if (scene) {
			m_pDX11Renderer->Render(scene);
		}
	}

	// FPS計測
	void CGameSystem::MeasFPS() {
		// 現在の時間を取得
		m_nowTime = timeGetTime();
		m_wfps++;

		if (m_nowTime - m_oldTime >= 1000) {
			m_fps = m_wfps;
			m_wfps = 0;
			m_oldTime = m_nowTime;
		}
	}

} // namespace