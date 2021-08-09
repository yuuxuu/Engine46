/**
 * @file CSpotLight.h
 * @brief
 * @author 木村優
 * @date 2021/08/02
 */

#pragma once

#include "CLight.h"

namespace Engine46 {

	class CSpotLight : public CLight {
	private:
		float m_angle;

	public:
		explicit CSpotLight(const char* lightName);
		~CSpotLight();

	};

} // namespace


