/**
 * @file CCamera.cpp
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#include "CCamera.h"

namespace Engine46 {

	// コンストラクタ
	CCamera::CCamera(const int sWidth, const int sHeight) :
		CActorBase((int)ClassType::Camera, "Camera", Transform()),
		m_Eye(VECTOR3(0.0f, 0.0f, -10.0f)),
		m_Focus(),
		m_UP(VECTOR3(0.0f, 1.0f, 0.0f))
	{
		m_matProj.dx_m = DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(90.0f),
			(float)sWidth / (float)sHeight,
			Z_NEAR,
			Z_FAR
		);
	}

	// デストラクタ
	CCamera::~CCamera()
	{}

	// 初期化
	void CCamera::Initialize() {

		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CCamera, m_Eye), sizeof(m_Eye)));
		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CCamera, m_Focus), sizeof(m_Focus)));
		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CCamera, m_UP), sizeof(m_UP)));
	}

	// 更新
	void CCamera::Update() {
		DirectX::XMVECTOR eye = DirectX::XMVectorSet(m_Eye.x, m_Eye.y, m_Eye.z, 0.0f);
		DirectX::XMVECTOR focus = DirectX::XMVectorSet(m_Focus.x, m_Focus.y, m_Focus.z, 0.0f);
		DirectX::XMVECTOR up = DirectX::XMVectorSet(m_UP.x, m_UP.y, m_UP.z, 0.0f);

		m_matView.dx_m = DirectX::XMMatrixLookAtLH(eye, focus, up);

		m_Transform.pos = m_Eye;
	}

	// ViewProjection合成行列を取得
	Matrix CCamera::GetVPMatrix() {
		Matrix matVP;
		matVP.dx_m = m_matView.dx_m * m_matProj.dx_m;
		
		return matVP;
	}

} // namespace