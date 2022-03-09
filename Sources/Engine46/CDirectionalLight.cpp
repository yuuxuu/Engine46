/**
 * @file CDirectionalLight.cpp
 * @brief
 * @author 木村優
 * @date 2021/08/01
 */

#include "CDirectionalLight.h"

namespace Engine46 {

    // コンストラクタ
    CDirectionalLight::CDirectionalLight(const std::string& lightName) :
        CLight(UINT(LightType::Directional), lightName),
        m_intensity(0)
    {}

    // デストラクタ
    CDirectionalLight::~CDirectionalLight()
    {}
}
