/**
 * @file CTimer.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/10
 */

#include "CTimer.h"

namespace Engine46 {

    // コンストラクタ
    CTimer::CTimer() :
        m_nowTime(0),
        m_oldTime(timeGetTime())
    {}

    // デストラクタ
    CTimer::~CTimer()
    {
        m_nowTime = timeGetTime();

        std::cout << (float)(m_nowTime - m_oldTime) / 1000.0f << "[ms]" << std::endl;
    }

    // 更新
    void CTimer::Update() {
        // 現在の時間を取得
        m_nowTime = timeGetTime();

        if (m_nowTime - m_oldTime >= 1000.0f) {
            m_oldTime = m_nowTime;

            std::cout << (float)(m_nowTime - m_oldTime) / 1000.0f << "[ms]" << std::endl;
        }
    }

} // namespace