/**
 * @file CSpotLight.h
 * @brief
 * @author 木村優
 * @date 2021/08/02
 */

#include "CSpotLight.h"

namespace Engine46 {

    // コンストラクタ
    CSpotLight::CSpotLight(const std::string& lightName) :
        CLight(UINT(LightType::Spot), lightName)
    {}

    // デストラクタ
    CSpotLight::~CSpotLight()
    {}

} // namespace
