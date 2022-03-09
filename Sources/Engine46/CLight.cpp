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

namespace Engine46 {

    // コンストラクタ
    CLight::CLight(const UINT lightType, const std::string& lightName) :
        CActorBase(int(ActorType::Light), lightName, Transform()),
        m_lightType(lightType),
        m_lightID(0),
        m_lightDiffuse(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_lightSpecular(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_lightAmbinet(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_lightEmissive(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_lightAttenuation(VECTOR4(0.1f, 0.1f, 0.0f, 1.0f)),
        m_degree(0),
        m_r(0.0f)
    {}

    // デストラクタ
    CLight::~CLight()
    {}

    // 更新
    void CLight::Update() 
    {}

} // namespace
