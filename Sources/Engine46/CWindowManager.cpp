//*****************************************************************************
//!	@file	CWindowManager.cpp
//!	@brief	
//!	@note	CWindowManager�N���X
//!	@author 2019/09/26 �쐬�F�ؑ��D
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

	// �R���X�g���N�^
	CWindowManager::CWindowManager() :
		m_hInstance(nullptr),
		m_WinMode(0)
	{}

	// �f�X�g���N�^
	CWindowManager::~CWindowManager() 
	{}

	// �X�V
	void CWindowManager::Update() {

	}

	// �E�C���h�E�̍쐬	
	bool CWindowManager::CreateCWindow(HINSTANCE hInstance, std::unique_ptr<CWindow> pWindow) {
		if (!pWindow->InitInstance(hInstance)) {
			std::cout << "�C���X�^���X�̓o�^:���s" << std::endl;
			return false;
		}

		// �E�C���h�E�̏�����
		if (!pWindow->InitWindow(hInstance, m_WinMode)) {
			std::cout << "�E�C���h�E������:���s" << std::endl;
			return false;
		}

		m_pVecCWindow.emplace_back(move(pWindow));

		return true;
	}

	// �쐬
	void CWindowManager::Create() {
		if (!m_pCWinowManager) m_pCWinowManager = new CWindowManager;
	}

	// �J��
	void CWindowManager::Release() {
		if (m_pCWinowManager) delete m_pCWinowManager; m_pCWinowManager = nullptr;
	}

} // namespace