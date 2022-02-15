/**
 * @file CThreadPoolSystem.cpp
 * @brief
 * @author 木村優
 * @date 2022/02/07
 */

#include "CThreadPoolSystem.h"
#include "CGameSystem.h"

namespace Engine46 {

    // コンストラクタ
    CThreadPoolSystem::CThreadPoolSystem() 
    {}

    // デストラクタ
    CThreadPoolSystem::~CThreadPoolSystem() {
        Finalize();
    }
    
    // タスク処理
    void CThreadPoolSystem::WorkTask() {
        CoInitialize(NULL);
        for (;;) {
            std::function<void()> workTask;

            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_conditionVariable.wait(lock, [&] { return !m_queueWorkTask.empty() || !m_isRunning; });

                if (!m_isRunning && m_queueWorkTask.empty()) {
                    break;
                }

                workTask = std::move(m_queueWorkTask.front());
                m_queueWorkTask.pop();
            }

            workTask();
        }
        CoUninitialize();
    }

    // タスクを追加
    void CThreadPoolSystem::AddWorkTask(const std::function<void()>& workTask) {
        const std::lock_guard<std::mutex> lock(m_mutex);

        if (!m_isRunning) {
            throw std::runtime_error("Cannot schedule new task after shutdown");
        }

        m_queueWorkTask.push(workTask);

        m_conditionVariable.notify_one();
    }

    // 初期化
    void CThreadPoolSystem::Initialize(UINT numThread) {

        std::cout << "std::thread::hardware_concurrency() = " << std::thread::hardware_concurrency() << std::endl;

        for (UINT i = 0; i < numThread; ++i) {
            m_vecThread.push_back(std::make_unique<std::thread>(&CThreadPoolSystem::WorkTask, this));
        }

        m_isRunning = true;
    }

    // 終了
    void CThreadPoolSystem::Finalize() {
        if (!m_isRunning) return;

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_isRunning = false;
        }

        m_conditionVariable.notify_all();

        int num = static_cast<int>(m_vecThread.size());
        int cntNum = 0;
        for (;;) {
            for (const auto& thread : m_vecThread) {
                if (thread.get()->joinable()) {
                    thread.get()->join();
                    cntNum++;
                }
            }
            if (num == cntNum) break;
        }
    }

} // namespace