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
        m_lightAttenuation(VECTOR4(0.0f, 0.0f, 0.1f, 1.0f)),
        m_degree(0),
        m_r(0.0f)
    {
        std::random_device rd;
        std::mt19937 mt(rd());

        std::uniform_int rand_degree(0, 360);

        m_degree = rand_degree(mt);

        std::uniform_real_distribution<float> randR(0.0f, 1000.0f);

        m_r = randR(mt);
    }

    // デストラクタ
    CLight::~CLight()
    {}

    // 更新
    void CLight::Update() {
        float degree = float(++m_degree % 360);

        float rad = DegreeToRadian(degree);

        m_transform.pos.x = m_r * cosf(rad);
        m_transform.pos.z = m_r * sinf(rad);
    }

} // namespace
