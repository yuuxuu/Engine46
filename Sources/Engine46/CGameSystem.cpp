/**
 * @file CGameSystem.h
 * @brief
 * @author 木村優
 * @date 2018/12/19
 */
#include "CGameSystem.h"
#include "CActorManager.h"

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
		GameSystemExit();
	}

	// ゲームシステムの初期化
	bool CGameSystem::GameSystemInit(HINSTANCE hInstance) {
		// コンソール呼び出し
		CallConsole();
		// 乱数生成
		srand((unsigned)time(NULL));
		// ロケール設定
		setlocale(LC_CTYPE, "");
		// タイマの分解能力を１ｍｓにする
		timeBeginPeriod(1);
		// メインウインドウ作成
		m_mainWindow = std::make_unique<CWindow>("MainWindow", "Engine46");
		// メインウインドウ初期化
		if (!m_mainWindow->InitWindow(hInstance)) {
			std::cout << "ウインドウ初期化:失敗" << std::endl;
			return false;
		}
		// ゲームメインスレッド生成
		m_gameSystemThread = std::thread(&CGameSystem::GameSystemLoop, this);
		if (!m_gameSystemThread.joinable()) {
			std::cout << "ゲームメインスレッド生成:失敗" << std::endl;
			return false;
		}

		CActorManager manager;
		//std::unique_ptr<CObject> object1 = std::make_unique<CObject>(0, "object1111", Transform());
		//std::unique_ptr<CObject> object2 = std::make_unique<CObject>(0, "object2222", Transform());
		//manager.AddObjectToObjectList(object1);
		//manager.AddObjectToObjectList(object2);
		//manager.SaveObjectList();
		manager.LoadActorList();

		return true;
	}

	// ゲームシステムメイン
	void CGameSystem::GameSystemLoop() {
		// イベントハンドル生成
		m_hGame = CreateEvent(NULL, false, false, NULL);
		if (!m_hGame) {
			MessageBox(NULL, "CGamesSYSTEM::CreateEventエラー", "MessageBox", MB_OK);
			return;
		}

		DWORD sts;
		while (1) {
			// 1000ms(1秒)待つ
			sts = WaitForSingleObject(m_hGame, 1000);
			if (sts == WAIT_FAILED) {
				break;
			}

			GameSystemUpdate();

			GameSystemDraw();

			MeasFPS();
		}
	}
	// ゲームシステムの更新
	void CGameSystem::GameSystemUpdate() {

	}
	// ゲームシステム描画
	void CGameSystem::GameSystemDraw() {

	}
	// ゲームシステムの終了
	void CGameSystem::GameSystemExit() {
		if (m_hGame) {
			CloseHandle(m_hGame);
			m_hGame = 0;
		}
		// ゲームメインスレッドの終了を待つ
		if (m_gameSystemThread.joinable()) {
			m_gameSystemThread.join();
		}

		FreeConsole();
		// タイマの分解能力を元に戻す
		timeEndPeriod(1);
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