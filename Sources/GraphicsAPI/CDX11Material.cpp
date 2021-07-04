/**
 * @file CDX11Material.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/22
 */

#include "CDX11Material.h"
#include "CDX11Device.h"
#include "CDX11DeviceContext.h"
#include "CDX11ConstantBuffer.h"

namespace Engine46 {

	struct materialCB {
		VECTOR4	diffuse;
		VECTOR4	specular;
		VECTOR4	ambient;
		VECTOR4	emissive;
		VECTOR4	brightness;
	};

	// コンストラクタ
	CDX11Material::CDX11Material(CDX11Device* pDevice, CDX11DeviceContext* pDeviceContext) :
		pDX11Device(pDevice),
		pDX11DeviceContext(pDeviceContext)
	{}

	// デストラクタ
	CDX11Material::~CDX11Material()
	{}

	// 作成
	void CDX11Material::Create() {

		if (!m_pConstantBuffer) {
			m_pConstantBuffer = std::make_unique<CDX11ConstantBuffer>(pDX11Device, pDX11DeviceContext);

			m_pConstantBuffer->CreateConstantBuffer(sizeof(materialCB));

			this->Update();
		}
	}

	// 更新
	void CDX11Material::Update() {
		materialCB cb = {
			m_diffuse,
			m_specular,
			m_ambient,
			m_emissive,
			m_brightness,
		};

		m_pConstantBuffer->Update(&cb);
	}

	// マテリアルをシェーダーへ設定
	void CDX11Material::Set(UINT slot) {
		if (pShaderPackage) {
			pShaderPackage->SetShader();
		}

		if (pTexture) {
			pTexture->Set(0);
		}

		if (m_pConstantBuffer) {
			m_pConstantBuffer->Set(slot);
		}
	}

} // namespace