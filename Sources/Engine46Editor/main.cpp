/**
 * @file main.cpp
 * @brief
 * @author 木村優
 * @date 2021/06/26
 */

#include "Engine46Editor.h"

#include "Engine46/CGameSystem.h"
#include "Engine46/CFileSystem.h"
#include "Engine46/CRendererSystem.h"

#include <QApplication>

#pragma comment(lib, "Engine46.lib")
#pragma comment(lib, "GraphicsAPI.lib")

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    Engine46Editor w;
    w.show();

    Engine46::CFileSystem& fileSystem = Engine46::CFileSystem::GetFileSystem();
    if (!fileSystem.Initialize()) {
        MessageBoxA(NULL, "ファイルシステム初期化：失敗", "MessageBox", MB_OK);
        return -1;
    }

    QWidget* pRenderWidget = w.GetRenderWidget();
    HWND hwnd = (HWND)pRenderWidget->winId();
    Engine46::RECT rect = Engine46::RECT(pRenderWidget->width(), pRenderWidget->height());

    Engine46::CRendererSystem& rendererSystem = Engine46::CRendererSystem::GetRendererSystem();
    if (!rendererSystem.Initialize(hwnd, rect)) {
        MessageBoxA(NULL, "レンダラーシステム初期化：失敗", "MessageBox", MB_OK);
        return -1;
    }

    hwnd = (HWND)w.winId();

    Engine46::CGameSystem& gameSystem = Engine46::CGameSystem::GetGameSystem();
    if (!gameSystem.Initialize(rendererSystem.GetRenderer(), hwnd)) {
        MessageBoxA(NULL, "ゲームシステム初期化：失敗", "MessageBox", MB_OK);
        return -1;
    }

    w.InitializeEditor();

    return a.exec();
}
