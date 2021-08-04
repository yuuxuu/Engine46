/**
 * @file CPointLight.cpp
 * @brief
 * @author 木村優
 * @date 2021/08/02
 */

#include "CPointLight.h"

namespace Engine46 {

	// コンストラクタ
	CPointLight::CPointLight(const char* lightName) :
		CLight(lightName),
		m_radius(0)
	{}

	// デストラクタ
	CPointLight::~CPointLight()
	{}

} // namespace
