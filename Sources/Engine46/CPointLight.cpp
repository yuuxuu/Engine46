/**
 * @file CPointLight.cpp
 * @brief
 * @author 木村優
 * @date 2021/08/02
 */

#include "CPointLight.h"

namespace Engine46 {

    // コンストラクタ
    CPointLight::CPointLight(const std::string& lightName) :
        CLight(UINT(LightType::Point), lightName),
        m_radius(0)
    {
        std::random_device rd;
        std::mt19937 mt(rd());

        std::uniform_real_distribution<float> rand_degree(0.0f, 360.0f);
        std::uniform_real_distribution<float> rand_r(0.0f, 3000.0f);
        std::uniform_int ran_turn(0, 1);
        std::uniform_real_distribution<float> rand_speed(0.1f, 1.5f);

        m_degree = rand_degree(mt);
        m_r = rand_r(mt);
        m_turn = ran_turn(mt);
        m_speed = rand_speed(mt);
    }

    // デストラクタ
    CPointLight::~CPointLight()
    {}

    // 更新
    void CPointLight::Update() {
        m_degree += (m_turn == 0) ? m_speed : -m_speed;

        float rad = DegreeToRadian(m_degree);

        if (m_turn == 0 && rad > PI || m_turn != 0 && rad < -PI) {
            m_degree *= -1;
        }

        m_transform.pos.x = m_r * cosf(rad);
        m_transform.pos.z = m_r * sinf(rad);
    }

} // namespace
