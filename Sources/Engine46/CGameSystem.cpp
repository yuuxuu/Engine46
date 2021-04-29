/**
 * @file CGameSystem.h
 * @brief
 * @author �ؑ��D
 * @date 2018/12/19
 */
#include "CGameSystem.h"
#include "CObjectManager.h"

namespace Engine46 {

	// �R���X�g���N�^
	CGameSystem::CGameSystem() :
		m_hGame(nullptr),
		m_mainWindow(nullptr),
		m_fps(0),
		m_wfps(0),
		m_oldTime(0),
		m_nowTime(0) 
	{}

	// �f�X�g���N�^
	CGameSystem::~CGameSystem() {
		GameSystemExit();
	}

	// �Q�[���V�X�e���̏�����
	bool CGameSystem::GameSystemInit(HINSTANCE hInstance) {
		// �R���\�[���Ăяo��
		CallConsole();
		// ��������
		srand((unsigned)time(NULL));
		// ���P�[���ݒ�
		setlocale(LC_CTYPE, "");
		// �^�C�}�̕���\�͂��P�����ɂ���
		timeBeginPeriod(1);
		// ���C���E�C���h�E�쐬
		m_mainWindow = std::make_unique<CWindow>("MainWindow", "Engine46");
		// ���C���E�C���h�E������
		if (!m_mainWindow->InitWindow(hInstance)) {
			std::cout << "�E�C���h�E������:���s" << std::endl;
			return false;
		}
		// �Q�[�����C���X���b�h����
		m_gameSystemThread = std::thread(&CGameSystem::GameSystemLoop, this);
		if (!m_gameSystemThread.joinable()) {
			std::cout << "�Q�[�����C���X���b�h����:���s" << std::endl;
			return false;
		}

		CObjectManager manager;
		//std::unique_ptr<CObject> object1 = std::make_unique<CObject>(0, "object1111", Transform());
		//std::unique_ptr<CObject> object2 = std::make_unique<CObject>(0, "object2222", Transform());
		//manager.AddObjectToObjectList(object1);
		//manager.AddObjectToObjectList(object2);
		//manager.SaveObjectList();
		manager.LoadObjectList();

		return true;
	}

	// �Q�[���V�X�e�����C��
	void CGameSystem::GameSystemLoop() {
		// �C�x���g�n���h������
		m_hGame = CreateEvent(NULL, false, false, NULL);
		if (!m_hGame) {
			MessageBox(NULL, "CGamesSYSTEM::CreateEvent�G���[", "MessageBox", MB_OK);
			return;
		}

		DWORD sts;
		while (1) {
			// 1000ms(1�b)�҂�
			sts = WaitForSingleObject(m_hGame, 1000);
			if (sts == WAIT_FAILED) {
				break;
			}

			GameSystemUpdate();

			GameSystemDraw();

			MeasFPS();
		}
	}
	// �Q�[���V�X�e���̍X�V
	void CGameSystem::GameSystemUpdate() {

	}
	// �Q�[���V�X�e���`��
	void CGameSystem::GameSystemDraw() {

	}
	// �Q�[���V�X�e���̏I��
	void CGameSystem::GameSystemExit() {
		if (m_hGame) {
			CloseHandle(m_hGame);
			m_hGame = 0;
		}
		// �Q�[�����C���X���b�h�̏I����҂�
		if (m_gameSystemThread.joinable()) {
			m_gameSystemThread.join();
		}

		FreeConsole();
		// �^�C�}�̕���\�͂����ɖ߂�
		timeEndPeriod(1);
	}

	// FPS�v��
	void CGameSystem::MeasFPS() {
		// ���݂̎��Ԃ��擾
		m_nowTime = timeGetTime();
		m_wfps++;

		if (m_nowTime - m_oldTime >= 1000) {
			m_fps = m_wfps;
			m_wfps = 0;
			m_oldTime = m_nowTime;
		}
	}

} // namespace