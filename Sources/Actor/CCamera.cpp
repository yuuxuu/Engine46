/**
 * @file CCamera.cpp
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#include "CCamera.h"

namespace Engine46 {

	constexpr float Z_NEAR = 0.1f;
	constexpr float Z_FAR = 1000.0f;

	// コンストラクタ
	CCamera::CCamera(const int sWidth, const int sHeight) :
		CActorBase((int)ClassType::Camera, "Camera", Transform()),
		m_eye(0.0f, 0.0f, -10.0f),
		m_forcus(),
		m_up(0.0f, 1.0f, 0.0f),
		m_speed(1.0f)
	{
		m_matProj.dx_m = DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(90.0f),
			(float)sWidth / (float)sHeight,
			Z_NEAR,
			Z_FAR);
	}

	// デストラクタ
	CCamera::~CCamera()
	{}

	// 初期化
	void CCamera::Initialize() {

		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CCamera, m_eye), sizeof(m_eye)));
		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CCamera, m_forcus), sizeof(m_forcus)));
		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CCamera, m_up), sizeof(m_up)));
		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CCamera, m_matView), sizeof(m_matView)));
		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CCamera, m_matProj), sizeof(m_matProj)));
	}

	// 更新
	void CCamera::Update() {
		if (pInput) {
			m_speed = this->GetCameraSpeed(m_speed);

			VECTOR3 forward = GetCameraForwardVector() * m_speed;
			VECTOR3 right = GetCameraRightVector() * m_speed;

			if(pInput->IsPressKey(DIK_W)) {
				m_eye += forward;
			}
			if (pInput->IsPressKey(DIK_S)) {
				m_eye -= forward;
			}
			if (pInput->IsPressKey(DIK_A)) {
				m_eye -= right;
			}
			if (pInput->IsPressKey(DIK_D)) {
				m_eye += right;
			}

			if (pInput->IsTriggerLeftMouse()) {
				const float speed = 0.1f;

				m_forcus.x += pInput->GetMousePosX() * speed;
				m_forcus.y += pInput->GetMousePosY() * speed;
			}
		}

		DirectX::XMVECTOR eye = { m_eye.x, m_eye.y, m_eye.z };
		DirectX::XMVECTOR forcus = { m_forcus.x, m_forcus.y, m_forcus.z };
		DirectX::XMVECTOR up = { m_up.x, m_up.y, m_up.z };

		m_matView.dx_m = DirectX::XMMatrixLookAtLH(eye, forcus, up);
	}

	// ビュー行列とプロジェクション行列を合成し取得
	Matrix CCamera::GetViewProjectionMatrix() {
		Matrix matVP;
		matVP.dx_m = m_matView.dx_m * m_matProj.dx_m;
		
		return matVP;
	}

	// カメラのスピードを取得
	float CCamera::GetCameraSpeed(float nowSpeed) {
		const float minSpeed = 1.0f;
		const float maxSpeed = 3.0f;
		
		float raito = pInput->GetMousePosZ();
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