/**
 * @file CRendererSystem.cpp
 * @brief
 * @author 木村優
 * @date 2021/06/20
 */

#include "CRendererSystem.h"
#include "CGameSystem.h"

#include "CScene.h"

#include "GraphicsAPI/DirectX11/CDX11Renderer.h"
#include "GraphicsAPI/DirectX12/CDX12Renderer.h"

namespace Engine46 {

    // コンストラクタ
    CRendererSystem::CRendererSystem() :
        m_isInitialize(false)
    {}

    // デストラクタ
    CRendererSystem::~CRendererSystem() {
        Finalize();
    }

    // 初期化
    bool CRendererSystem::Initialize(HWND hwnd, RECT rect) {

        if (m_isInitialize) return true;

        m_pRenderer = std::make_unique<CDX12Renderer>();
        if (!m_pRenderer->Initialize(hwnd, rect.w, rect.h)) return false;

        // イベントハンドル生成
        m_hRenderer = CreateEvent(NULL, false, false, NULL);
        if (!m_hRenderer) {
            MessageBox(NULL, "CRendererSystem::CreateEventエラー", "MessageBox", MB_OK);
            return false;
        }
        // レンダラーメインスレッド生成
        m_rendererSystemThread = std::thread(&CRendererSystem::Loop, this);
        if (!m_rendererSystemThread.joinable()) {
            std::cout << "レンダラーメインスレッド生成:失敗" << std::endl;
            return false;
        }

        m_isInitialize = true;

        return true;
    }

    // 終了
    void CRendererSystem::Finalize() {

        if (m_hRenderer) {
            CloseHandle(m_hRenderer);
            m_hRenderer = 0;
        }

        // レンダラースレッドの終了待ち
        if (m_rendererSystemThread.joinable()) {
            m_rendererSystemThread.join();
        }

        if (m_pRenderer) {
            m_pRenderer.release();
        }
    }

    // ループ
    void CRendererSystem::Loop() {
        while (1) {
            if (CGameSystem::GetGameSystem().IsInitialize()) break;
        }

        DWORD sts;
        DWORD ms = 1000 / 60; // 1000ms/60spd = 0.16ms

        while (1) {
            sts = WaitForSingleObject(m_hRenderer, ms);
            if (sts == WAIT_FAILED) break;

            Update();
        }
    }

    // 描画
    void CRendererSystem::Update() {
        if (!m_pRenderer || !pRenderScene) return;

        m_pRenderer->Begine(pRenderScene);
        m_pRenderer->Render(pRenderScene);
    }

} // namespace
