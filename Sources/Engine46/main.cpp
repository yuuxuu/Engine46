/**
 * @file main.cpp
 * @brief
 * @author 木村優
 * @date 2018/12/15
 */

#include "CGameSystem.h"

using namespace Engine46;

// コンソールの呼び出し
void CallConsole()
{
    char Console[128];
    HWND ConsoleWindow;
    ::RECT ConsoleWindowRect;

    FILE* pfile = NULL; // ファイルポインタ

    // コンソール出力
    AllocConsole();
    // ストリームと結ぶ
    freopen_s(&pfile, "CONOUT$", "w", stdout);
    freopen_s(&pfile, "CONNIN$", "r", stdin);

    // コンソールタイトル検索
    GetConsoleTitle(Console, sizeof(Console));
    // コンソールウインドウハンドル取得
    ConsoleWindow = FindWindow(NULL, Console);
    // 現在のコンソールウインドウ位置を取得
    GetWindowRect(ConsoleWindow, &ConsoleWindowRect);
    // コンソールウインドウ位置変更
    MoveWindow(ConsoleWindow, 0, 0, ConsoleWindowRect.right - ConsoleWindowRect.left, ConsoleWindowRect.bottom - ConsoleWindowRect.top, TRUE);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgs, int nWinMode)
{
    CallConsole();

    MSG	msg;
    while (1) {
        if (!GetMessage(&msg, NULL, 0, 0)) {
            break;
        }
        else {
            // 文字メッセージへのコンバート
            TranslateMessage(&msg);
            // メッセージをWndProcへ送る
            DispatchMessage(&msg);
        }
    }

    FreeConsole();

    return (int)msg.wParam;
}