/**
 * @file CThreadPoolSystem.h
 * @brief
 * @author 木村優
 * @date 2022/02/07
 */

#pragma once

#ifndef _CTASK_SYTEM_H_
#define _CTASK_SYTEM_H_

namespace Engine46 {

    class CThreadPoolSystem {
    private:
        std::vector<std::unique_ptr<std::thread>>   m_vecThread;

        std::queue<std::function<void()>>           m_queueWorkTask;

        std::atomic<bool>                           m_isRunning;

        std::condition_variable                     m_conditionVariable;

        std::mutex                                  m_mutex;



    private:
        CThreadPoolSystem();
        ~CThreadPoolSystem();

        void WorkTask();

    public:

        void Initialize(UINT numThread = std::thread::hardware_concurrency());
        void Finalize();

        void AddWorkTask(const std::function<void()>& workTask);

        static CThreadPoolSystem& GetThreadPoolSystem() {
            static CThreadPoolSystem threadPoolSystem;
            return threadPoolSystem;
        }

        UINT GetNumThread() const { return static_cast<UINT>(m_vecThread.size()); }
    };

} // namespace

#endif

