/**
 * @file CCamera.cpp
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#include "CCamera.h"
#include "CInput.h"
#include "CConstantBuffer.h"

namespace Engine46 {

    // コンストラクタ
    CCamera::CCamera(const std::string& actorName, const int sWidth, const int sHeight) :
        CActorBase(int(ActorType::Camera), actorName, Transform()),
        m_eye(0.0f, 0.0f, -10.0f),
        m_forcus(),
        m_up(0.0f, 1.0f, 0.0f),
        m_speed(1.0f)
    {
        m_matProj.dx_m = DirectX::XMMatrixPerspectiveFovLH(
            DegreeToRadian(90.0f),
            (float)sWidth / (float)sHeight,
            Z_NEAR,
            Z_FAR);
    }

    // デストラクタ
    CCamera::~CCamera()
    {}

    // 初期化
    void CCamera::Initialize() 
    {}

    // 更新
    void CCamera::Update() {
        if (m_pInput) {
            m_pInput->UpdateInput();

            m_speed = this->GetCameraSpeed(m_speed);

            VECTOR3 forward = GetCameraForwardVector() * m_speed;
            VECTOR3 right = GetCameraRightVector() * m_speed;
            VECTOR3 up = GetCameraUpVector() * m_speed;

            if(m_pInput->IsPressKey(DIK_W)) {
                m_eye += forward;
                m_forcus += forward;
            }
            if (m_pInput->IsPressKey(DIK_S)) {
                m_eye -= forward;
                m_forcus -= forward;
            }
            if (m_pInput->IsPressKey(DIK_A)) {
                m_eye -= right;
                m_forcus -= right;
            }
            if (m_pInput->IsPressKey(DIK_D)) {
                m_eye += right;
                m_forcus += right;
            }

            const float speed = 0.01f;
            if (m_pInput->IsTriggerLeftMouse()) {

                m_forcus += right * m_pInput->GetMousePosX() * speed;
                m_forcus += up * m_pInput->GetMousePosY() * speed;
            }
            if (m_pInput->IsTriggerRightMouse()) {
                float r = Vec3Lenght(m_forcus - m_eye);

                float theta = m_transform.rotate.y + m_pInput->GetMousePosY() * speed;
                float phi = m_transform.rotate.x + m_pInput->GetMousePosX() * speed;

                VECTOR3 pos;
                pos.x = r * sinf(theta) * cosf(phi);
                pos.y = r * cosf(theta);
                pos.z = r * sinf(theta) * sinf(phi);

                m_eye = m_forcus + pos;

                m_transform.rotate.x = phi;
                m_transform.rotate.y = theta;
            }
        }

        DirectX::XMVECTOR eye = { m_eye.x, m_eye.y, m_eye.z };
        DirectX::XMVECTOR forcus = { m_forcus.x, m_forcus.y, m_forcus.z };
        DirectX::XMVECTOR up = { m_up.x, m_up.y, m_up.z };

        m_matView.dx_m = DirectX::XMMatrixLookAtLH(eye, forcus, up);

        m_transform.pos = m_eye;
    }

    // ビュー行列とプロジェクション行列を合成し取得
    Matrix CCamera::GetViewProjectionMatrix() {
        Matrix matVP;
        matVP.dx_m = m_matView.dx_m * m_matProj.dx_m;
        
        return matVP;
    }

    // ビュー逆行列を取得
    Matrix CCamera::GetInvViewMatrix() {
        Matrix matView;
        matView.dx_m = DirectX::XMMatrixInverse(nullptr, m_matView.dx_m);

        matView._41 = matView._42 = matView._43 = 0.0f;

        return matView;
    }

    // プロジェクション逆行列を取得
    Matrix CCamera::GetInvProjectionMatrix() {
        Matrix matProj;
        matProj.dx_m = DirectX::XMMatrixInverse(nullptr, m_matProj.dx_m);

        return matProj;
    }

    // カメラのスピードを取得
    float CCamera::GetCameraSpeed(float nowSpeed) {
        const float minSpeed = 1.0f;
        const float maxSpeed = 10.0f;
        
        float raito = m_pInput->GetMousePosZ();
        if (raito < 0.1f && raito > -0.1f) {
            return nowSpeed;
        }

        raito = (raito / std::fabsf(raito)) * 0.1f;

        nowSpeed += raito;
        if (nowSpeed < minSpeed) {
            return minSpeed;
        }
        if (nowSpeed > maxSpeed) {
            return maxSpeed;
        }

        return nowSpeed;
    }

    // カメラの右向きベクトルを取得
    VECTOR3 CCamera::GetCameraRightVector() {
        return VECTOR3(m_matView._11, m_matView._21, m_matView._31);
    }

    // カメラの上向きベクトルを取得
    VECTOR3 CCamera::GetCameraUpVector() {
        return VECTOR3(m_matView._12, m_matView._22, m_matView._32);
    }

    // カメラの前向きベクトルを取得
    VECTOR3 CCamera::GetCameraForwardVector() {
        return VECTOR3(m_matView._13, m_matView._23, m_matView._33);
    }

} // namespace