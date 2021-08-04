/**
 * @file CPointLight.h
 * @brief
 * @author 木村優
 * @date 2021/08/02
 */

#pragma once

#include "CLight.h"

namespace Engine46 {

	class CPointLight : public CLight {
	private:
		float m_radius;

	public:
		explicit CPointLight(const char* lightName);
		~CPointLight();
	};

} // namespace


