/**
 * @file main.cpp
 * @brief
 * @author 木村優
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

	// コンソール呼び出し
	CallConsol();
	// 乱数生成
	srand((unsigned)time(NULL));
	// ロケール設定
	setlocale(LC_CTYPE, "");

	// ウインドウマネージャーを作成
	CWindowManager::Create();
	CWindowManager::Get()->CreateCWindow(hInstance, std::make_unique<CWindow>("MainWindow", "Engine46"));

	//// ファイルシステムの作成
	//CFileSystemManager::Create();
	//// ファイルシステムの初期化
	//if (!CFileSystemManager::Get()->InitFileSystem()) {
	//	MessageBox(NULL, "ファイルシステムの作成及び初期化:失敗", "MessageBox", MB_OK);
	//	// ファイルシステムの終了
	//	CFileSystemManager::Release();
	//}
	//else {
	//	std::cout << "ファイルシステムの作成及び初期化:完了" << std::endl;
	//}

	//// ゲームシステムの作成
	//CGameSystemManager::Create();
	//// ゲームシステムの初期化
	//if (!CGameSystemManager::Get()->GameSystemInit()) {
	//	MessageBox(NULL, "ゲームシステムの作成及び初期化:失敗", "MessageBox", MB_OK);
	//	// ゲームシステムの終了
	//	CGameSystemManager::Release();
	//	return -1;
	//}
	//else {
	//	std::cout << "ゲームシステムの作成及び初期化:完了" << std::endl;
	//}

	// イベントタイマーをセットする
	timeBeginPeriod(1);									// タイマの分解能力を１ｍｓにする
	// ゲームシステムIDの取得
	int gameSystemId = timeSetEvent(16, 1, GameSystemManagerProc, 1, TIME_PERIODIC);
	// ファイルシステムIDの取得
	int fileSystemId = timeSetEvent(16, 1, FileSystemManagerProc, 1, TIME_PERIODIC);

	while (1) {											// メッセージ･ループ
		if (!GetMessage(&msg, NULL, 0, 0)) {			// メッセージを取得
			break;
		}
		else {
			TranslateMessage(&msg); 					// 文字メッセージへのコンバート
			DispatchMessage(&msg); 						// メッセージをWndProcへ送る
		}
	}

	if (gameSystemId) timeKillEvent(gameSystemId);	// マルチメディアタイマの終了
	if (fileSystemId) timeKillEvent(fileSystemId);	// マルチメディアタイマの終了

	timeEndPeriod(1);									// タイマの分解能力もとに戻す

	//CFileSystemManager::Release();						// ファイルシステムの終了
	//CGameSystemManager::Release();						// ゲームシステムの終了
	//CWindowManager::Release();							// ウインドウマネージャーの開放

	FreeConsole();										// コンソールの開放
	return (int)msg.wParam;
}

void CALLBACK GameSystemManagerProc(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	//if (CGameSystemManager::Get()->GetOnMultiThread()) {
	//	// マルチスレッド用
	//	CGameSystemManager::SetGameEvent();								// ゲームシステムイベントセット
	//}
	//else {
	//	// シングルスレッド用
	//	CGameSystemManager::Get()->GameSystemUpdate();					// ゲームシステムの更新
	//	CGameSystemManager::Get()->GameSystemDraw();					// ゲームシステムの描画
	//}
}

void CALLBACK FileSystemManagerProc(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	//CFileSystemManager::SetFileEvent();			// ファイルシステムイベントのセット
}

// コンソールの呼び出し
void CallConsol()
{
	char Consol[128];
	HWND ConsolWindow;
	RECT ConsolWindowRect;

	FILE* pfile = NULL; // ファイルポインタ

	// コンソール出力
	AllocConsole();
	// ストリームと結ぶ
	freopen_s(&pfile, "CONOUT$", "w", stdout);
	freopen_s(&pfile, "CONNIN$", "r", stdin);

	// コンソールタイトル検索
	GetConsoleTitle(Consol, sizeof(Consol));
	// コンソールウインドウハンドル取得
	ConsolWindow = FindWindow(NULL, Consol);
	// 現在のコンソールウインドウ位置を取得
	GetWindowRect(ConsolWindow, &ConsolWindowRect);
	// コンソールウインドウ位置変更
	MoveWindow(ConsolWindow, 0, 0, ConsolWindowRect.right - ConsolWindowRect.left, ConsolWindowRect.bottom - ConsolWindowRect.top, TRUE);
}