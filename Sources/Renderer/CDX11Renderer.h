/**
 * @file CDX11Renderer.h
 * @brief
 * @author 木村優
 * @date 2021/05/02
 */

#pragma once

#ifndef _CDX11RENDERER_H_
#define _CDX11RENDERER_H_

#include "IRenderer.h"

namespace Engine46 {

	// 前方宣言
	class CDX11ForwardRendering;

	class CDX11Renderer : public IRenderer {
	private:
		ComPtr<IDXGISwapChain>					m_pSwapChain;

		ComPtr<ID3D11Device>					m_pDevice;

		ComPtr<ID3D11DeviceContext>				m_pDeviceContext;

		ComPtr<ID3D11RenderTargetView>			m_pRtv;

		std::unique_ptr<CDX11ForwardRendering>	m_pDX11FRendering;

		size_t									m_iLayoutBufSize;

	public:
		CDX11Renderer();
		~CDX11Renderer();

		bool Initialize(HWND hwnd, UINT width, UINT height) override;
		
		void Finalize() override;

		bool Render() override;

		bool CreateTexture2D(ComPtr<ID3D11Texture2D>& pTex2D, D3D11_TEXTURE2D_DESC& texDesc, D3D11_SUBRESOURCE_DATA* pInitData = nullptr);
		bool CreateRenderTargetView(ComPtr<ID3D11RenderTargetView>& pRtv, ID3D11Texture2D* pTex2D, D3D11_RENDER_TARGET_VIEW_DESC& rtvDesc);
		bool CreateShaderResourceView(ComPtr<ID3D11ShaderResourceView>& pSrv, ID3D11Texture2D* pTex2D, D3D11_SHADER_RESOURCE_VIEW_DESC& srvDesc);
		bool CreateDepthStencilView(ComPtr<ID3D11DepthStencilView>& pDsv, ID3D11Texture2D* pTex2D, D3D11_DEPTH_STENCIL_VIEW_DESC& dsvDesc);

		void ClearRenderTargetView(ID3D11RenderTargetView* pSrv);
		void ClearDespthStencilView(ID3D11DepthStencilView* pDsv, UINT clearFlags);
		
		void SetRenderTargetView(ID3D11RenderTargetView* pSrv, ID3D11DepthStencilView* pDsv);
		void SetRenderTargetViews(std::vector<ComPtr<ID3D11RenderTargetView>>& vecRtv, ID3D11DepthStencilView* pDsv);

		void SetInputLayout(void* pBuf, size_t bufSize);
		void SetSamplerState(D3D11_SAMPLER_DESC& sDesc, UINT slot);
		void SetBlendState(D3D11_BLEND_DESC& bDesc);
		void SetRasterizerState(D3D11_RASTERIZER_DESC& rsDesc);
		void SetDepthStencilState(D3D11_DEPTH_STENCIL_DESC& dsDesc, UINT Ref = 0x00);

		void SetViewPort(int width, int height);
	};

} // namespace

#endif