/**
 * @file CCamera.h
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#pragma once

#ifndef _CCAMERA_H_
#define _CCAMERA_H_

#include "CActor.h"

namespace Engine46 {

	constexpr UINT PLANE_MAX	= 6;
	constexpr float Z_NEAR		= 0.1f;
	constexpr float Z_FAR		= 1000.0f;

	struct  PLANE {
		// 平面の方程式
		float a, b, c, d;
		float Len;
	};

	class CCamera : public CActorBase {
	private:
		VECTOR3 m_Eye;
		VECTOR3 m_Focus;
		VECTOR3 m_UP;

		Matrix	m_matView;
		Matrix  m_matProj;

	public:
		CCamera(const int sWidth, const int sHeight);
		~CCamera();

		void Initialize() override;
		void Update() override;

		Matrix GetVPMatrix();
	};

} // namespace

#endif