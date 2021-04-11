/**
 * @file main.cpp
 * @brief
 * @author �ؑ��D
 * @date 2018/12/15
 */

#include "main.h"
#include "CWindowManager.h"
//#include	"CGameSystemManager.h"
//#include	"CFileSystemManager.h"
//#include	"CGuiManager.h"
//#include	"CRenderManager.h"

using namespace Engine46;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgs,int nWinMode)
{
	MSG	msg;
	bool flag = true;

	// �R���\�[���Ăяo��
	CallConsol();
	// ��������
	srand((unsigned)time(NULL));
	// ���P�[���ݒ�
	setlocale(LC_CTYPE, "");

	// �E�C���h�E�}�l�[�W���[���쐬
	CWindowManager::Create();
	CWindowManager::Get()->CreateCWindow(hInstance, std::make_unique<CWindow>("MainWindow", "Engine46"));

	//// �t�@�C���V�X�e���̍쐬
	//CFileSystemManager::Create();
	//// �t�@�C���V�X�e���̏�����
	//if (!CFileSystemManager::Get()->InitFileSystem()) {
	//	MessageBox(NULL, "�t�@�C���V�X�e���̍쐬�y�я�����:���s", "MessageBox", MB_OK);
	//	// �t�@�C���V�X�e���̏I��
	//	CFileSystemManager::Release();
	//}
	//else {
	//	std::cout << "�t�@�C���V�X�e���̍쐬�y�я�����:����" << std::endl;
	//}

	//// �Q�[���V�X�e���̍쐬
	//CGameSystemManager::Create();
	//// �Q�[���V�X�e���̏�����
	//if (!CGameSystemManager::Get()->GameSystemInit()) {
	//	MessageBox(NULL, "�Q�[���V�X�e���̍쐬�y�я�����:���s", "MessageBox", MB_OK);
	//	// �Q�[���V�X�e���̏I��
	//	CGameSystemManager::Release();
	//	return -1;
	//}
	//else {
	//	std::cout << "�Q�[���V�X�e���̍쐬�y�я�����:����" << std::endl;
	//}

	// �C�x���g�^�C�}�[���Z�b�g����
	timeBeginPeriod(1);									// �^�C�}�̕���\�͂��P�����ɂ���
	// �Q�[���V�X�e��ID�̎擾
	int gameSystemId = timeSetEvent(16, 1, GameSystemManagerProc, 1, TIME_PERIODIC);
	// �t�@�C���V�X�e��ID�̎擾
	int fileSystemId = timeSetEvent(16, 1, FileSystemManagerProc, 1, TIME_PERIODIC);

	while (1) {											// ���b�Z�[�W����[�v
		if (!GetMessage(&msg, NULL, 0, 0)) {			// ���b�Z�[�W���擾
			break;
		}
		else {
			TranslateMessage(&msg); 					// �������b�Z�[�W�ւ̃R���o�[�g
			DispatchMessage(&msg); 						// ���b�Z�[�W��WndProc�֑���
		}
	}

	if (gameSystemId) timeKillEvent(gameSystemId);	// �}���`���f�B�A�^�C�}�̏I��
	if (fileSystemId) timeKillEvent(fileSystemId);	// �}���`���f�B�A�^�C�}�̏I��

	timeEndPeriod(1);									// �^�C�}�̕���\�͂��Ƃɖ߂�

	//CFileSystemManager::Release();						// �t�@�C���V�X�e���̏I��
	//CGameSystemManager::Release();						// �Q�[���V�X�e���̏I��
	//CWindowManager::Release();							// �E�C���h�E�}�l�[�W���[�̊J��

	FreeConsole();										// �R���\�[���̊J��
	return (int)msg.wParam;
}

void CALLBACK GameSystemManagerProc(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	//if (CGameSystemManager::Get()->GetOnMultiThread()) {
	//	// �}���`�X���b�h�p
	//	CGameSystemManager::SetGameEvent();								// �Q�[���V�X�e���C�x���g�Z�b�g
	//}
	//else {
	//	// �V���O���X���b�h�p
	//	CGameSystemManager::Get()->GameSystemUpdate();					// �Q�[���V�X�e���̍X�V
	//	CGameSystemManager::Get()->GameSystemDraw();					// �Q�[���V�X�e���̕`��
	//}
}

void CALLBACK FileSystemManagerProc(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	//CFileSystemManager::SetFileEvent();			// �t�@�C���V�X�e���C�x���g�̃Z�b�g
}

// �R���\�[���̌Ăяo��
void CallConsol()
{
	char Consol[128];
	HWND ConsolWindow;
	RECT ConsolWindowRect;

	FILE* pfile = NULL; // �t�@�C���|�C���^

	// �R���\�[���o��
	AllocConsole();
	// �X�g���[���ƌ���
	freopen_s(&pfile, "CONOUT$", "w", stdout);
	freopen_s(&pfile, "CONNIN$", "r", stdin);

	// �R���\�[���^�C�g������
	GetConsoleTitle(Consol, sizeof(Consol));
	// �R���\�[���E�C���h�E�n���h���擾
	ConsolWindow = FindWindow(NULL, Consol);
	// ���݂̃R���\�[���E�C���h�E�ʒu���擾
	GetWindowRect(ConsolWindow, &ConsolWindowRect);
	// �R���\�[���E�C���h�E�ʒu�ύX
	MoveWindow(ConsolWindow, 0, 0, ConsolWindowRect.right - ConsolWindowRect.left, ConsolWindowRect.bottom - ConsolWindowRect.top, TRUE);
}