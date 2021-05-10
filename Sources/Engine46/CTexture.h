/**
 * @file CTexture.h
 * @brief
 * @author 木村優
 * @date 2021/05/07
 */

#pragma once

#ifndef _CTEXTURE_H_
#define _CTEXTURE_H_

#include "math.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	// 前方宣言
	class CDX11Renderer;

	class CTextureBase {
	protected:
		const char*		m_Name;

		const char*		m_pData;
		size_t			m_dataRowPitch;
		size_t			m_dataSlicePitch;

	public:
		CTextureBase();
		virtual ~CTextureBase();

		bool Initialize(const char* name, const char* pData, size_t rowPitch, size_t slicePitch);

		virtual void Create() {};
		virtual void Set(UINT slot) {};
	};

	class CDX11Texture : public CTextureBase {
	private:
		CDX11Renderer*						pDX11Renderer;

		ComPtr<ID3D11Texture2D>				m_pTex2D;
		ComPtr<ID3D11ShaderResourceView>	m_pSrv;

		D3D11_TEXTURE2D_DESC				m_texDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC		m_srvDesc;

	public:
		CDX11Texture(CDX11Renderer* pRenderer, D3D11_TEXTURE2D_DESC& texDesc, D3D11_SHADER_RESOURCE_VIEW_DESC& srvDesc);
		~CDX11Texture();

		void Create() override;
		void Set(UINT slot) override;

		ID3D11Texture2D* GetTexture2D() const { return m_pTex2D.Get(); }
	};

} // namespace

#endif