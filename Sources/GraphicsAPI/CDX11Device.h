/**
 * @file CDX11Device.h
 * @brief
 * @author 木村優
 * @date 2021/05/22
 */

#pragma once

#ifndef _CDX11DEVICE_H_
#define _CDX11DEVICE_H_

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	class CDX11Device {
	private:
		ComPtr<IDXGISwapChain>			m_pSwapChain;

		ComPtr<ID3D11Device>			m_pDevice;

	public:
		CDX11Device();
		~CDX11Device();

		bool Initialize(ComPtr<ID3D11DeviceContext>& m_pDeviceContext, HWND hwnd, UINT width, UINT height);

		bool GetBackBuffer(ID3D11Texture2D*& pTex2D);

		bool Present();

		bool CreateBuffer(ComPtr<ID3D11Buffer>& pBuffer, D3D11_BUFFER_DESC& bufDesc, D3D11_SUBRESOURCE_DATA* pInitData = nullptr);

		bool CreateTexture2D(ComPtr<ID3D11Texture2D>& pTex2D, D3D11_TEXTURE2D_DESC& texDesc, D3D11_SUBRESOURCE_DATA* pInitData = nullptr);
		
		bool CreateRenderTargetView(ComPtr<ID3D11RenderTargetView>& pRtv, ID3D11Texture2D* pTex2D, D3D11_RENDER_TARGET_VIEW_DESC& rtvDesc);
		bool CreateShaderResourceView(ComPtr<ID3D11ShaderResourceView>& pSrv, ID3D11Texture2D* pTex2D, D3D11_SHADER_RESOURCE_VIEW_DESC& srvDesc);
		bool CreateDepthStencilView(ComPtr<ID3D11DepthStencilView>& pDsv, ID3D11Texture2D* pTex2D, D3D11_DEPTH_STENCIL_VIEW_DESC& dsvDesc);

		bool CreateInputLayout(ComPtr<ID3D11InputLayout>& pILayout, void* pBuf, size_t bufSize);
		bool CreateSamplerState(ComPtr<ID3D11SamplerState>& pSstate, D3D11_SAMPLER_DESC& sDesc);
		bool CreateBlendState(ComPtr<ID3D11BlendState>& pBState, D3D11_BLEND_DESC& bDesc);
		bool CreateRasterizerState(ComPtr<ID3D11RasterizerState>& pRSstate, D3D11_RASTERIZER_DESC& rsDesc);
		bool CreateDepthStencilState(ComPtr<ID3D11DepthStencilState>& pDSstate, D3D11_DEPTH_STENCIL_DESC& dsDesc);

		bool CreateVertexShader(ComPtr<ID3D11VertexShader>& pVS, ID3DBlob* pBlob);
		bool CreatePixelShader(ComPtr<ID3D11PixelShader>& pPS, ID3DBlob* pBlob);
		bool CreateHullShader(ComPtr<ID3D11HullShader>& pHS, ID3DBlob* pBlob);
		bool CreateDomainShader(ComPtr<ID3D11DomainShader>& pDS, ID3DBlob* pBlob);
		bool CreateGeometryShader(ComPtr<ID3D11GeometryShader>& pGS, ID3DBlob* pBlob);
		bool CreateComputeShader(ComPtr<ID3D11ComputeShader>& pCS, ID3DBlob* pBlob);
	};

} // namespace

#endif
