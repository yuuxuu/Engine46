//*****************************************************************************
//!	@file	CWindowManager.cpp
//!	@brief	
//!	@note	CWindowManagerクラス
//!	@author 2019/09/26 作成：木村優
//*****************************************************************************

#include	"CWindowManager.h"
//#include	"CDirectXShowManager.h"
//#include	"CGuiManager.h"
//#include	"CSceneManager.h"
//#include	"CCamera.h"
//#include	"CSkyBox.h"

//extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Engine46 {

	CWindowManager* CWindowManager::m_pCWinowManager = nullptr;

	// コンストラクタ
	CWindowManager::CWindowManager() :
		m_hInstance(nullptr),
		m_WinMode(0)
	{}

	// デストラクタ
	CWindowManager::~CWindowManager() 
	{}

	// 更新
	void CWindowManager::Update() {

	}

	// ウインドウの作成	
	bool CWindowManager::CreateCWindow(HINSTANCE hInstance, std::unique_ptr<CWindow> pWindow) {
		if (!pWindow->InitInstance(hInstance)) {
			std::cout << "インスタンスの登録:失敗" << std::endl;
			return false;
		}

		// ウインドウの初期化
		if (!pWindow->InitWindow(hInstance, m_WinMode)) {
			std::cout << "ウインドウ初期化:失敗" << std::endl;
			return false;
		}

		m_pVecCWindow.emplace_back(move(pWindow));

		return true;
	}

	// 作成
	void CWindowManager::Create() {
		if (!m_pCWinowManager) m_pCWinowManager = new CWindowManager;
	}

	// 開放
	void CWindowManager::Release() {
		if (m_pCWinowManager) delete m_pCWinowManager; m_pCWinowManager = nullptr;
	}

} // namespace