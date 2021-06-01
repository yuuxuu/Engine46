/**
 * @file CDX11Texture.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/22
 */

#include "CDX11Texture.h"
#include "CDX11Device.h"
#include "CDX11DeviceContext.h"

namespace Engine46 {

	// コンストラクタ
	CDX11Texture::CDX11Texture(CDX11Device* pDevice, CDX11DeviceContext* pDeviceContext, const char* name) :
		CTextureBase(name),
		pDX11Device(pDevice),
		pDX11DeviceContext(pDeviceContext)
	{}

	// デストラクタ
	CDX11Texture::~CDX11Texture()
	{}

	// テクスチャ2D作成
	void CDX11Texture::Create(D3D11_TEXTURE2D_DESC& texDesc, D3D11_SHADER_RESOURCE_VIEW_DESC& srvDesc) {

		D3D11_SUBRESOURCE_DATA subData = {};
		if (m_textureData.pData) {
			subData.pSysMem = m_textureData.pData.get();
			subData.SysMemPitch = m_textureData.rowPitch;
			subData.SysMemSlicePitch = m_textureData.slicePitch;

			pDX11Device->CreateTexture2D(m_pTex2D, texDesc, &subData);
		}
		else {
			pDX11Device->CreateTexture2D(m_pTex2D, texDesc, nullptr);
		}

		pDX11Device->CreateShaderResourceView(m_pSrv, m_pTex2D.Get(), srvDesc);
	}

	// テクスチャ2D作成
	void CDX11Texture::Create() {

		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width				= m_textureData.width;
		texDesc.Height				= m_textureData.height;
		texDesc.MipLevels			= 1;
		texDesc.ArraySize			= 1;
		texDesc.Format				= m_textureData.format;
		texDesc.SampleDesc.Count	= 1;
		texDesc.SampleDesc.Quality	= 0;
		texDesc.Usage				= D3D11_USAGE_DEFAULT;
		texDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags		= 0;
		texDesc.MiscFlags			= 0;

		D3D11_SUBRESOURCE_DATA subData = {};
		if (m_textureData.pData) {
			subData.pSysMem				= m_textureData.pData.get();
			subData.SysMemPitch			= m_textureData.rowPitch;
			subData.SysMemSlicePitch	= m_textureData.slicePitch;

			pDX11Device->CreateTexture2D(m_pTex2D, texDesc, &subData);
		}
		else {
			pDX11Device->CreateTexture2D(m_pTex2D, texDesc, nullptr);
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.ViewDimension		= D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Format				= texDesc.Format;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

		pDX11Device->CreateShaderResourceView(m_pSrv, m_pTex2D.Get(), srvDesc);
	}

	// テクスチャをシェーダーに設定
	void CDX11Texture::Set(UINT slot) {
		pDX11DeviceContext->SetPSShaderResources(slot, 1, m_pSrv.Get());
	}

} // namespace