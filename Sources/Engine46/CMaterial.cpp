/**
 * @file CMaterial.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/07
 */

#include "CMaterial.h"

#include "../Renderer/CDX11Renderer.h"

#include "../Engine46/CConstantBuffer.h"

namespace Engine46 {

	struct materialCB {
		VECTOR4	diffuse;
		VECTOR4	specular;
		VECTOR4	ambient;
		VECTOR4	emissive;
		VECTOR4	brightness;
	};

	// コンストラクタ
	CMaterialBase::CMaterialBase() :
		m_diffuse(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_specular(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_ambient(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_emissive(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_brightness(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		pTexture(nullptr)
	{}

	// デストラクタ
	CMaterialBase::~CMaterialBase()
	{}

	// コンストラクタ
	CDX11Material::CDX11Material(CDX11Renderer* pRenderer) :
		pDX11Renderer(pRenderer)
	{}

	// デストラクタ
	CDX11Material::~CDX11Material()
	{}

	// 作成
	void CDX11Material::CreateConstantBuffer() {

		if (!m_pConstantBuffer) {
			m_pConstantBuffer = std::make_unique<CDX11CB>(pDX11Renderer);
		}

		m_pConstantBuffer->CreateConstantBuffer(sizeof(materialCB));

		this->Update();
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
		
		m_pConstantBuffer->Set(slot);
	}

} // namespace