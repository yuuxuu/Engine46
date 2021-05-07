/**
 * @file CTexture.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/07
 */

#include "CTexture.h"

#include "../Renderer/CDX11Renderer.h"

namespace Engine46 {

	// コンストラクタ
	CTextureBase::CTextureBase() :
		m_Name(),
		m_pData(),
		m_dataRowPitch(),
		m_dataSlicePitch()
	{}

	// デストラクタ
	CTextureBase::~CTextureBase()
	{}

	// 初期化
	bool CTextureBase::Initialize(const char* name, const char* pData, size_t rowPitch, size_t slicePitch) {

		m_Name = name;
		m_pData = pData;
		m_dataRowPitch = rowPitch;
		m_dataSlicePitch = slicePitch;

		return true;
	}

	// コンストラクタ
	CDX11Texture::CDX11Texture(
		CDX11Renderer* pRenderer,
		D3D11_TEXTURE2D_DESC& texDesc,
		D3D11_SHADER_RESOURCE_VIEW_DESC& srvDesc) :
		
		pDX11Renderer(pRenderer),
		m_texDesc(texDesc),
		m_srvDesc(srvDesc)
	{}

	// デストラクタ
	CDX11Texture::~CDX11Texture()
	{}

	// テクスチャ2D作成
	void CDX11Texture::Create() {

		D3D11_SUBRESOURCE_DATA subData = {};
		if (m_pData) {
			subData.pSysMem				= m_pData;
			subData.SysMemPitch			= m_dataRowPitch;
			subData.SysMemSlicePitch	= 0;
		}

		pDX11Renderer->CreateTexture2D(m_pTex2D, m_texDesc, &subData);

		pDX11Renderer->CreateShaderResourceView(m_pSrv, m_pTex2D.Get(), m_srvDesc);
	}

	// テクスチャをシェーダーに設定
	void CDX11Texture::Set(UINT slot) {
		pDX11Renderer->SetPSShaderResources(slot, 1, m_pSrv.Get());
	}

} // namespace