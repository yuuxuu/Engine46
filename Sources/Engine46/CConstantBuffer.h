/**
 * @file CConstantBuffer.h
 * @brief
 * @author 木村優
 * @date 2021/05/07
 */

#pragma once

#ifndef _CCONSTANT_BUFFER_H_
#define _CCONSTANT_BUFFER_H_

#include "math.h"

namespace Engine46 {

	enum class CB_TYPE {
		WORLD,
		CAMERA,
		MATERIAL,
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT,
	};

	constexpr int LIGHT_MAX = 1024 / 2;

	struct DirectionalLightCB {
		VECTOR3 pos;
		float	dummy;
		VECTOR4 diffuse;
		VECTOR4 specular;
	};

	struct PointLightCB {
		struct PointLight {
			VECTOR3 pos;
			float	radius;
			VECTOR4 diffuse;
			VECTOR4 specular;
			VECTOR4 attenuation;
		};

		PointLightCB() :
			numPointLight(0)
		{}

		PointLight pointLights[LIGHT_MAX];
		int numPointLight;
		int dummy1;
		int dummy2;
		int dummy3;
	};

	struct SpotLightCB {
		struct SpotLight {
			VECTOR3 pos;
			float	angle;
			VECTOR4 diffuse;
			VECTOR4 specular;
			VECTOR4 attenuation;
		};

		SpotLightCB() :
			numSpotLight(0)
		{}

		SpotLight spotLights[LIGHT_MAX];
		int numSpotLight;
		int dummy1;
		int dummy2;
		int dummy3;
	};

	class CConstantBufferBase {
	protected:

	public:
		CConstantBufferBase();
		virtual ~CConstantBufferBase();

		virtual void CreateConstantBuffer(UINT byteWidth) {};
		virtual void Update(void* srcData) {};
		virtual void Set(UINT slot) {};
	};

} // namespace

#endif
