/**
 * @file CWinowManager.cpp
 * @brief
 * @author 木村優
 * @date 2019/09/28
 */

#include "CWinow.h"
#include "utility.h"

namespace Engine46 {

	UINT g_screenShotCount = 0;

	// コンストラクタ
	CWindow::CWindow(const char* windowClassName, const char* titleName):
		m_hwnd(nullptr),
		m_wcex(),
		m_pWindowClassName(windowClassName),
		m_pTitleName(titleName),
		m_windowWidth(0),
		m_windowHeight(0),
		m_clientWidth(0),
		m_clientHeight(0),
		m_onFullScreen(false),
		m_onScreenShot(false)
	{
		if (IDYES == MessageBox(NULL, "フルスクリーンで起動しますか？", "MessageBox", MB_YESNO)) {
			m_onFullScreen = true;
			m_windowWidth = FULL_SCREEN_X;
			m_windowHeight = FULL_SCREEN_Y;
		}
		else {
			m_windowWidth = SCREEN_X;
			m_windowHeight = SCREEN_Y;
		}
	}

	// デストラクタ
	CWindow::~CWindow() {
		UnregisterClass(m_wcex.lpszClassName, m_wcex.hInstance);
	}

	// ウインドウプロシージャ
	LRESULT CWindow::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

		//ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam);

		switch (message) {
		case WM_CREATE:
			// ファイルのドロップを許可
			DragAcceptFiles(hwnd, true);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_MOUSEWHEEL:
			break;
		case WM_KEYDOWN:
			switch (wParam) {
			case VK_ESCAPE:
				if (IDYES == MessageBox(NULL, "終了しますか？", "MessageBox", MB_YESNO)) {
					DestroyWindow(hwnd);
				}
				break;
			}
			break;
		case WM_DROPFILES:
		{
			// DROP構造体へのハンドル
			HDROP hDrop;
			char ch[256] = {};
			UINT fileSize = 0;

			hDrop = (HDROP)wParam;

			// ドロップされたファイル数を検索
			fileSize = DragQueryFile(hDrop, -1, NULL, 0);

			for (auto i = 0; i < (int)fileSize; ++i) {

				// ドロップされたファイルのファイルパスを取得
				DragQueryFile(hDrop, i, ch, sizeof(ch));

				//string str = ConvertString(ch);

				//CFileSystemManager::Get()->SetDrop(str);
			}

			//CFileSystemManager::Get()->SetLoadingFileType(FILE_TYPE::DROP_FILE);

			// ファイルの開放
			if (hDrop) DragFinish(hDrop);
		}
		break;
		default:

			break;
		}

		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	// インスタンスの初期化
	bool CWindow::InitInstance(HINSTANCE hInstance) {

		m_wcex.hInstance		= hInstance;									// インスタンス値のセット
		m_wcex.lpszClassName	= m_pWindowClassName;							// クラス名
		m_wcex.lpfnWndProc		= WindowProc;									// ウインドウメッセージ関数
		m_wcex.style			= CS_HREDRAW | CS_VREDRAW;						// ウインドウスタイル
		m_wcex.cbSize			= sizeof(WNDCLASSEX);							// 構造体のサイズ
		m_wcex.hIcon			= LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);	// ラージアイコン
		m_wcex.hIconSm			= LoadIcon((HINSTANCE)NULL, IDI_WINLOGO);		// スモールアイコン
		m_wcex.hCursor			= LoadCursor((HINSTANCE)NULL, IDC_ARROW);		// カーソルスタイル
		m_wcex.lpszMenuName		= NULL; 										// メニューなし
		m_wcex.cbClsExtra		= 0;											// エキストラなし
		m_wcex.cbWndExtra		= 0;
		m_wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);			// 背景色白

		if (!RegisterClassEx(&m_wcex)) return false;							// ウインドウクラスの登録

		return true;
	}

	// ウインドウの初期化
	bool CWindow::InitWindow(HINSTANCE hInstance, int nWinMode) {
		RECT rClient;

		m_hwnd = CreateWindow(
			m_pWindowClassName,				// ウィンドウクラスの名前
			m_pTitleName,					// タイトル
			WS_VISIBLE | WS_SYSMENU,		// ウィンドウスタイル
			0,								// ウィンドウ位置_縦
			0,								// ウィンドウ位置_横
			m_windowWidth,					// ウィンドウ横幅
			m_windowHeight,					// ウィンドウ縦幅
			NULL,							// 親ウィンドウなし
			NULL,							// メニューなし
			hInstance,						// インスタンスハンドル
			NULL);							// 追加引数なし

		if (!m_hwnd) return false;
		else { std::cout << "ウインドウ初期化:完了" << std::endl; }

		GetClientRect(m_hwnd, &rClient);

		m_clientWidth = m_windowWidth - (rClient.right - rClient.left);
		m_clientHeight = m_windowHeight - (rClient.bottom - rClient.top);

		m_windowWidth -= m_clientWidth;
		m_windowHeight -= m_clientHeight;

		// ウインドウを表示する
		if (!ShowWindow(m_hwnd, SW_SHOW)) return false;
		if (!UpdateWindow(m_hwnd)) return false;

		return true;
	}

	// ウインドウのサイズ変更
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

	//	ウインドウをスクリーンショット
	void CWindow::ScreenShotWindow() {

		BYTE* pBuf = GetBuffer();

		if (pBuf) {

			if (m_onScreenShot) {

				std::string savePath = "Assets/Texture/ScreenShot/ScreenShot_" + std::to_string(g_screenShotCount) + ".bmp";

				BITMAPINFO bitmapInfo = {};

				bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				bitmapInfo.bmiHeader.biWidth = m_windowWidth;
				bitmapInfo.bmiHeader.biHeight = m_windowHeight;
				bitmapInfo.bmiHeader.biPlanes = 1;
				bitmapInfo.bmiHeader.biBitCount = 32;
				bitmapInfo.bmiHeader.biCompression = BI_RGB;

				SaveToBmpFile(bitmapInfo.bmiHeader, pBuf, savePath);

				g_screenShotCount++;

				m_onScreenShot = false;
			}
		}
	}

	// バッファの取得
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