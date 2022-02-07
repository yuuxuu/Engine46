/**
 * @file CTaskSystem.h
 * @brief
 * @author 木村優
 * @date 2022/02/07
 */

#pragma once

#ifndef _CTASK_SYTEM_H_
#define _CTASK_SYTEM_H_

namespace Engine46 {

    class CTaskSystem {
    private:
        std::thread                 m_taskSystemThread;
        HANDLE                      m_hTask;

        std::queue<std::thread>     m_queueWorkTask;

        bool                        m_isInitialize;

    private:
        CTaskSystem();
        ~CTaskSystem();

        void Loop();

    public:

        bool Initialize();
        void Finalize();

        void AddTask(std::thread& taskThread);

        static CTaskSystem& GetTaskSystem() {
            static CTaskSystem taskSystem;
            return taskSystem;
        }

    };

} // namespace

#endif

