/**
 * @file CMaterial.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/07
 */

#include "CMaterial.h"

#include "../Renderer/CDX11Renderer.h"

namespace Engine46 {

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
	void CDX11Material::Create() {

		UINT byteWidth = 0;
		byteWidth += sizeof(m_diffuse);
		byteWidth += sizeof(m_specular);
		byteWidth += sizeof(m_ambient);
		byteWidth += sizeof(m_emissive);
		byteWidth += sizeof(m_brightness);

		D3D11_BUFFER_DESC bufDesc = {};
		bufDesc.ByteWidth			= byteWidth;
		bufDesc.Usage				= D3D11_USAGE_DEFAULT;
		bufDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
		bufDesc.CPUAccessFlags		= 0;
		bufDesc.MiscFlags			= 0;
		bufDesc.StructureByteStride = 0;

		pDX11Renderer->CreateBuffer(m_pConstantBuffer, bufDesc);

		this->Update();
	}

	// 更新
	void CDX11Material::Update() {
		struct CB {
			VECTOR4	diffuse;
			VECTOR4	specular;
			VECTOR4	ambient;
			VECTOR4	emissive;
			VECTOR4	brightness;
		};
		CB cb = {
			m_diffuse,
			m_specular,
			m_ambient,
			m_emissive,
			m_brightness,
		};

		pDX11Renderer->UpdateSubResource(m_pConstantBuffer.Get(), &cb);
	}

	// マテリアルをシェーダーへ設定
	void CDX11Material::Set(UINT slot) {
		if (pTexture) {
			pTexture->Set(0);
		}
		
		pDX11Renderer->SetPSConstantBuffers(slot, 1, m_pConstantBuffer.Get());
	}

} // namespace