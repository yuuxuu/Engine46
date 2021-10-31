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
        CCamera(const char* actorName, const int sWidth, const int sHeight);
        ~CCamera();

        void InitializeResource(CRendererBase* pRenderer) override;

        void Initialize() override;
        void Update() override;

        Matrix GetViewProjectionMatrix();
        Matrix GetInvViewMatrix();

        VECTOR3 GetCameraRightVector();
        VECTOR3 GetCameraUpVector();
        VECTOR3 GetCameraForwardVector();
    };

} // namespace

#endif