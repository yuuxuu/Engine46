/**
 * @file CTaskSystem.cpp
 * @brief
 * @author 木村優
 * @date 2022/02/07
 */

#include "CTaskSystem.h"

namespace Engine46 {

    // コンストラクタ
    CTaskSystem::CTaskSystem() :
        m_hTask(nullptr),
        m_isInitialize(false)
    {}

    // デストラクタ
    CTaskSystem::~CTaskSystem()
    {}

    // 初期化
    bool CTaskSystem::Initialize() {

        if (m_isInitialize) return true;

        // イベントハンドル生成
        m_hTask = CreateEvent(NULL, false, false, NULL);
        if (!m_hTask) {
            MessageBox(NULL, "CTaskSystem::CrateEventエラー", "MessageBox", MB_OK);
            return false;
        }
        // タスクスレッド生成
        m_taskSystemThread = std::thread(&CTaskSystem::Loop, this);
        if (!m_taskSystemThread.joinable()) {
            std::cout << "タスクスレッド生成：失敗" << std::endl;
        }

        m_isInitialize = true;

        return true;
    }

    // 終了
    void CTaskSystem::Finalize() {

        if (m_hTask) {
            CloseHandle(m_hTask);
            m_hTask = 0;
        }
        // タスクスレッドの終了待ち
        if (m_taskSystemThread.joinable()) {
            m_taskSystemThread.join();
        }
    }

    // ワークタスクを追加
    void CTaskSystem::AddTask(std::thread& taskThread) {
        
        if (!m_isInitialize) {
            if (!Initialize()) {
                MessageBoxA(NULL, "タスクシステム初期化：失敗", "MessageBox", MB_OK);
                return;
            }
        }

        if (taskThread.joinable()) {
            m_queueWorkTask.push(std::move(taskThread));
        }
    }

    // ループ
    void CTaskSystem::Loop() {
        DWORD sts;
        DWORD ms = 1000 / 60;

        while (1) {
            sts = WaitForSingleObject(m_hTask, ms);
            if (sts == WAIT_FAILED) break;

            if (!m_queueWorkTask.empty()) {
                if (m_queueWorkTask.front().joinable()) {
                    m_queueWorkTask.front().join();
                    m_queueWorkTask.pop();
                }
            }
        }
    }

} // namespace