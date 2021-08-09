/**
 * @file CDirectionalLight.h
 * @brief
 * @author 木村優
 * @date 2021/08/01
 */

#pragma once

#include "CLight.h"

namespace Engine46 {

	class CDirectionalLight : public CLight {
	private:
		float m_intensity;

	public:
		explicit CDirectionalLight(const char* lightName);
		~CDirectionalLight();

	};

} // namespace

