/**
 * @file CLight.cpp
 * @brief
 * @author 木村優
 * @date 2021/07/04
 */

#include "CLight.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CConstantBuffer.h"
#include "CSprite.h"

namespace Engine46 {

    // コンストラクタ
    CLight::CLight(const char* lightName) :
        CActorBase((int)ActorType::Light, lightName, Transform()),
        m_lightID(0),
        m_lightDiffuse(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_lightSpecular(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_lightAmbinet(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_lightEmissive(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_lightAttenuation(VECTOR4(0.1f, 0.1f, 0.0f, 1.0f)),
        m_degree(0),
        m_r(0.0f)
    {
        std::random_device rd;
        std::mt19937 mt(rd());

        std::uniform_int rand_degree(0, 360);
        std::uniform_real_distribution<float> rand_r(0.0f, 3000.0f);
        std::uniform_int ran_turn(0, 1);
        std::uniform_real_distribution<float> rand_speed(0.1f, 1.5f);

        m_degree = rand_degree(mt);
        m_r = rand_r(mt);
        m_turn = ran_turn(mt);
        m_speed = rand_speed(mt);
    }

    // デストラクタ
    CLight::~CLight()
    {}

    // 更新
    void CLight::Update() {
        if (m_lightType == LightType::Directional) return;

        float degree = (m_turn == 0) ? float(++m_degree % 360) : float(--m_degree % 360);

        float rad = DegreeToRadian(degree * m_speed);

        m_transform.pos.x = m_r * cosf(rad);
        m_transform.pos.z = m_r * sinf(rad);
    }

} // namespace
