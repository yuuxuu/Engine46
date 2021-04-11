//*****************************************************************************
//!	@file	CWinowManager.cpp
//!	@brief	
//!	@note	CWinowManager�N���X
//!	@author 2019/09/28 �쐬�F�ؑ��D
//*****************************************************************************

#include "CWinow.h"
#include "utility.h"
//#include "CTextureManager.h"

namespace Engine46 {

	UINT g_screenshotCount = 0;

	// �R���X�g���N�^
	CWindow::CWindow(const char* windowClassName, const char* titleName):
		m_hwnd(nullptr),
		m_wcex(),
		m_windowClassName(windowClassName),
		m_titleName(titleName),
		m_windowWidth(0),
		m_windowHeight(0),
		m_onFullScreen(false),
		m_onScreenShot(false)
	{
		if (IDYES == MessageBox(NULL, "�t���X�N���[���ŋN�����܂����H", "MessageBox", MB_YESNO)) {
			m_onFullScreen = true;
			m_windowWidth = FULL_SCREEN_X;
			m_windowHeight = FULL_SCREEN_Y;
		}
		else {
			m_windowWidth = SCREEN_X;
			m_windowHeight = SCREEN_Y;
		}
	}

	// �f�X�g���N�^
	CWindow::~CWindow() {
		UnregisterClass(m_wcex.lpszClassName, m_wcex.hInstance);
	}

	// �E�C���h�E�v���V�[�W��
	LRESULT CWindow::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

		//ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam);

		switch (message) {
		case WM_CREATE:
			// �t�@�C���̃h���b�v������
			DragAcceptFiles(hwnd, true);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_MOUSEWHEEL:
		{
			//float r = 0;

			//CCamera* camera = dynamic_cast<CCamera*>(CSceneManager::Get()->GetObjectManager()->Get3DObject(OBJ_TYPE::CAMERA, 0));
			//// �}�E�X�z�C�[���̈ړ��ʂ��擾
			//r = camera->GetR() + GET_WHEEL_DELTA_WPARAM(wParam) * -2;
			//// �J�����̔��a���Z�b�g
			//if (r <= POSITION_MAX) camera->SetR(r);
			break;
		}
		case WM_KEYDOWN:
			switch (wParam) {
			case VK_ESCAPE:
				if (IDYES == MessageBox(NULL, "�I�����܂����H", "MessageBox", MB_YESNO)) {
					DestroyWindow(hwnd);
				}
				break;
			}
			break;
		case WM_DROPFILES:
		{
			// DROP�\���̂ւ̃n���h��
			HDROP hDrop;
			char ch[256] = {};
			UINT fileSize = 0;

			hDrop = (HDROP)wParam;

			// �h���b�v���ꂽ�t�@�C����������
			fileSize = DragQueryFile(hDrop, -1, NULL, 0);

			for (auto i = 0; i < (int)fileSize; ++i) {

				// �h���b�v���ꂽ�t�@�C���̃t�@�C���p�X���擾
				DragQueryFile(hDrop, i, ch, sizeof(ch));

				//string str = ConvertString(ch);

				//CFileSystemManager::Get()->SetDrop(str);
			}

			//CFileSystemManager::Get()->SetLoadingFileType(FILE_TYPE::DROP_FILE);

			// �t�@�C���̊J��
			if (hDrop) DragFinish(hDrop);
		}
		break;
		default:
			//long evcode;
			//long param1, param2;
			//if (CDirectXShowManager::Get()) {
			//	if (CDirectXShowManager::Get()->GetMediaEventEx()) {
			//		/*if (SUCCEEDED(CDirectXShowManager::Get()->GetMediaEventEx()->GetEvent(&evcode, &param1, &param2, 0))) {
			//			switch (evcode) {
			//			case EC_COMPLETE:
			//				CDirectXShowManager::Get()->SetMediaType(MEDIA_TYPE_::NONE);
			//				break;
			//			}
			//		}*/
			//	}
			//}
			break;
		}

		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	// �C���X�^���X�̏�����
	bool CWindow::InitInstance(HINSTANCE hInstance) {

		m_wcex.hInstance		= hInstance;									// �C���X�^���X�l�̃Z�b�g
		m_wcex.lpszClassName	= m_windowClassName;							// �N���X��
		m_wcex.lpfnWndProc		= WindowProc;									// �E�C���h�E���b�Z�[�W�֐�
		m_wcex.style			= CS_HREDRAW | CS_VREDRAW;						// �E�C���h�E�X�^�C��
		m_wcex.cbSize			= sizeof(WNDCLASSEX);							// �\���̂̃T�C�Y
		m_wcex.hIcon			= LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);	// ���[�W�A�C�R��
		m_wcex.hIconSm			= LoadIcon((HINSTANCE)NULL, IDI_WINLOGO);		// �X���[���A�C�R��
		m_wcex.hCursor			= LoadCursor((HINSTANCE)NULL, IDC_ARROW);		// �J�[�\���X�^�C��
		m_wcex.lpszMenuName		= NULL; 										// ���j���[�Ȃ�
		m_wcex.cbClsExtra		= 0;											// �G�L�X�g���Ȃ�
		m_wcex.cbWndExtra		= 0;
		m_wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);			// �w�i�F��

		if (!RegisterClassEx(&m_wcex)) return false;							// �E�C���h�E�N���X�̓o�^

		return true;
	}

	// �E�C���h�E�̏�����
	bool CWindow::InitWindow(HINSTANCE hInstance, int nWinMode) {
		RECT rClient;

		m_hwnd = CreateWindow(
			m_windowClassName,				// �E�B���h�E�N���X�̖��O
			m_titleName,					// �^�C�g��
			WS_VISIBLE | WS_SYSMENU,		// �E�B���h�E�X�^�C��
			0,								// �E�B���h�E�ʒu_�c
			0,								// �E�B���h�E�ʒu_��
			m_windowWidth,					// �E�B���h�E����
			m_windowHeight,					// �E�B���h�E�c��
			NULL,							// �e�E�B���h�E�Ȃ�
			NULL,							// ���j���[�Ȃ�
			hInstance,						// �C���X�^���X�n���h��
			NULL);							// �ǉ������Ȃ�

		if (!m_hwnd) return false;
		else { std::cout << "�E�C���h�E������:����" << std::endl; }

		GetClientRect(m_hwnd, &rClient);

		m_clientWidth = m_windowWidth - (rClient.right - rClient.left);
		m_clientHeight = m_windowHeight - (rClient.bottom - rClient.top);

		m_windowWidth -= m_clientWidth;
		m_windowHeight -= m_clientHeight;

		// �E�C���h�E��\������
		if (!ShowWindow(m_hwnd, SW_SHOW)) return false;
		if (!UpdateWindow(m_hwnd)) return false;

		return true;
	}

	// �E�C���h�E�̃T�C�Y�ύX
	void CWindow::ChangeSizeWindow(const int posx, const int posy, const int width, const int height) {
		RECT rClient;

		SetWindowPos(
			m_hwnd,
			NULL,
			posx,
			posy,
			width,
			height,
			SWP_NOZORDER);

		GetClientRect(m_hwnd, &rClient);

		m_windowWidth = width;
		m_windowHeight = height;

		m_clientWidth = m_windowWidth - (rClient.right - rClient.left);
		m_clientHeight = m_windowHeight - (rClient.bottom - rClient.top);

		m_windowWidth -= m_clientWidth;
		m_windowHeight -= m_clientHeight;
	}

	//	�E�C���h�E���X�N���[���V���b�g
	void CWindow::ScreenShotWindow() {

		BYTE* pBuf = GetBuffer();

		if (pBuf) {

			if (m_onScreenShot) {

				std::string savePath = "Assets/Texture/ScreenShot/ScreenShot_" + std::to_string(g_screenshotCount) + ".bmp";

				BITMAPINFO bitmapInfo = {};

				bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				bitmapInfo.bmiHeader.biWidth = m_windowWidth;
				bitmapInfo.bmiHeader.biHeight = m_windowHeight;
				bitmapInfo.bmiHeader.biPlanes = 1;
				bitmapInfo.bmiHeader.biBitCount = 32;
				bitmapInfo.bmiHeader.biCompression = BI_RGB;

				SaveToBmpFile(bitmapInfo.bmiHeader, pBuf, savePath);

				g_screenshotCount++;

				m_onScreenShot = false;
			}
			else {
				//CTextureManager::Get()->CreateCaptureTexture(m_pBufs.get(), m_windowWidth, m_windowHeight);
			}
		}
	}

	// �o�b�t�@�̎擾
	BYTE* CWindow::GetBuffer() {
		std::unique_ptr<BYTE[]>	pBufs = nullptr;

		HDC hdc = GetDC(m_hwnd);
		if (hdc) {
			HBITMAP hBmp = CreateCompatibleBitmap(hdc, m_windowWidth, m_windowHeight);
			HDC hdc_bmp = CreateCompatibleDC(hdc);
			SelectObject(hdc_bmp, hBmp);

			if (!BitBlt(hdc_bmp, 0, 0, m_windowWidth, m_windowHeight, hdc, 0, 0, SRCCOPY)) {
				std::cout << "Failed" << std::endl;
			}

			BITMAP bmp;
			GetObject(hBmp, sizeof(BITMAP), &bmp);

			int offsetByte = 4;

			BITMAPINFO bitmapInfo = {};

			bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bitmapInfo.bmiHeader.biWidth = m_windowWidth;
			bitmapInfo.bmiHeader.biHeight = m_windowHeight;
			bitmapInfo.bmiHeader.biPlanes = 1;
			bitmapInfo.bmiHeader.biBitCount = 32;
			bitmapInfo.bmiHeader.biCompression = BI_RGB;

			pBufs = std::make_unique<BYTE[]>(m_windowWidth * m_windowHeight * offsetByte);

			HDC hDC = GetDC(NULL);
			GetDIBits(hDC, hBmp, 0, m_windowHeight, pBufs.get(), &bitmapInfo, DIB_RGB_COLORS);

			ReleaseDC(NULL, hDC);

			DeleteDC(hdc_bmp);
			DeleteObject(hBmp);
		}
		ReleaseDC(m_hwnd, hdc);

		return pBufs.get();
	}

} // namespace