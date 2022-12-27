/**
 * @file main.cpp
 * @brief
 * @author 木村優
 * @date 2018/12/15
 */

#include "main .h"

#include "CGameSystem.h"
#include "CRendererSystem.h"
#include "CThreadPoolSystem.h"
#include "CWinow.h"

namespace Engine46 {
    void Initialize(HWND hwnd, int width, int height) {
        CallConsole();

        Engine46::CRendererSystem& renderSystem = Engine46::CRendererSystem::GetRendererSystem();
        if (!renderSystem.Initialize(hwnd, width, height)) {
            MessageBoxA(NULL, "レンダラーシステム初期化：失敗", "MessageBox", MB_OK);
        }

        Engine46::CThreadPoolSystem& threadPoolSystem = Engine46::CThreadPoolSystem::GetThreadPoolSystem();
        threadPoolSystem.Initialize(3);

        Engine46::CGameSystem& gameSystem = Engine46::CGameSystem::GetGameSystem();
        if (!gameSystem.Initialize(renderSystem.GetRenderer(), hwnd)) {
            MessageBoxA(NULL, "ゲームシステム初期化：失敗", "MessageBox", MB_OK);
        }
    }

    void Finalize() {
        Engine46::CThreadPoolSystem& threadPoolSystem = Engine46::CThreadPoolSystem::GetThreadPoolSystem();
        threadPoolSystem.Finalize();

        Engine46::CRendererSystem& renderSystem = Engine46::CRendererSystem::GetRendererSystem();
        renderSystem.Finalize();

        Engine46::CGameSystem& gameSystem = Engine46::CGameSystem::GetGameSystem();
        gameSystem.Finalize();

        FreeConsole();
    }

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
} // namespace

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgs, int nWinMode)
{
    Engine46::CallConsole();

    Engine46::CWindow window;
    if (!window.Initialize(hInstance, "Engine46", ""))
        return -1;

    Engine46::Initialize(window.GetHwnd(), window.GetWindowSize().w, window.GetWindowSize().h);

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

    Engine46::Finalize();

    FreeConsole();

    return (int)msg.wParam;
}