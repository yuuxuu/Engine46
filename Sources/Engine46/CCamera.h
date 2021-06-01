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

	class CCamera : public CActorBase {
	private:
		VECTOR3 m_eye;
		VECTOR3 m_forcus;
		VECTOR3 m_up;

		Matrix	m_matView;
		Matrix	m_matProj;

		float	m_speed;

		float GetCameraSpeed(float nowSpeed);

	public:
		CCamera(const int sWidth, const int sHeight);
		~CCamera();

		void Initialize() override;
		void Update() override;

		Matrix GetViewProjectionMatrix();

		VECTOR3 GetCameraRightVector();
		VECTOR3 GetCameraUpVector();
		VECTOR3 GetCameraForwardVector();
	};

} // namespace

#endif