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

    // 前方宣言
    class CInput;

    constexpr float Z_NEAR = 0.1f;
    constexpr float Z_FAR = 100000.0f;

    class CCamera : public CActorBase {
    private:
        VECTOR3                     m_eye;
        VECTOR3                     m_forcus;
        VECTOR3                     m_up;

        Matrix                      m_matView;
        Matrix                      m_matProj;

        float                       m_speed;

        std::unique_ptr<CInput>     m_pInput;

    private:
        float GetCameraSpeed(float nowSpeed);

    public:
        CCamera(const std::string& actorName, const int sWidth, const int sHeight);
        ~CCamera();

        void Initialize() override;
        void Update() override;

        Matrix GetViewMatrix() const { return m_matView; }
        Matrix GetProjectionMatrix() const { return m_matProj; }

        Matrix GetViewProjectionMatrix();
        Matrix GetInvViewMatrix();
        Matrix GetInvProjectionMatrix();

        VECTOR3 GetCameraRightVector();
        VECTOR3 GetCameraUpVector();
        VECTOR3 GetCameraForwardVector();
    };

} // namespace

#endif