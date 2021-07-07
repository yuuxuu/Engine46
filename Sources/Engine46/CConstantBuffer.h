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
		LIGHT,
	};

	constexpr int LIGHT_MAX = 1024 / 2;

	struct LightCB {
		VECTOR4 lightPos[LIGHT_MAX];
		VECTOR4 lightDiffuse[LIGHT_MAX];
		VECTOR4 lightSpecular[LIGHT_MAX];
		VECTOR4 lightAttenuation[LIGHT_MAX];

		int		lightNum;
		int		dumy_01;
		int		dumy_02;
		int		dumy_03;
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
