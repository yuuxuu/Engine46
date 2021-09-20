/**
 * @file CDX11DeviceContext.h
 * @brief
 * @author 木村優
 * @date 2021/05/22
 */

#pragma once

#ifndef _CDX11DEVICECONTEXT_H_
#define _CDX11DEVICECONTEXT_H_

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	class CDX11DeviceContext {
	private:
		ComPtr<ID3D11DeviceContext> m_pDeviceContext;

	public:
		explicit CDX11DeviceContext(ComPtr<ID3D11DeviceContext>& pDeviceContext);
		~CDX11DeviceContext();

		void SetBuffer(ID3D11Buffer* pVertexBuf, ID3D11Buffer* pIndexBuf, UINT strides, UINT offset);

		void DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY topology, UINT numIndexes);

		void ClearRenderTargetView(ID3D11RenderTargetView* pSrv);
		void ClearDespthStencilView(ID3D11DepthStencilView* pDsv, UINT clearFlags);

		void SetRenderTargetView(ID3D11RenderTargetView* pSrv, ID3D11DepthStencilView* pDsv);
		void SetRenderTargetViews(std::vector<ID3D11RenderTargetView*>& vecRtv, ID3D11DepthStencilView* pDsv);

		void SetVsShader(ID3D11VertexShader* pVs);
		void SetPsShader(ID3D11PixelShader* pPs);
		void SetHsShader(ID3D11HullShader* pPs);
		void SetDsShader(ID3D11DomainShader* pPs);
		void SetGsShader(ID3D11GeometryShader* pPs);
		void SetCsShader(ID3D11ComputeShader* pPs);

		void SetPSShaderResources(UINT slot, UINT num, ID3D11ShaderResourceView* pSrv);

		void SetVSConstantBuffers(UINT slot, UINT num, ID3D11Buffer* pBuf);
		void SetPSConstantBuffers(UINT slot, UINT num, ID3D11Buffer* pBuf);

		void UpdateSubResource(ID3D11Buffer* pBuf, void* pSrcData);

		void SetInputLayout(ID3D11InputLayout* pILayout);
		void SetSamplerState(ID3D11SamplerState* pSstate, UINT slot);
		void SetBlendState(ID3D11BlendState* pBState);
		void SetRasterizerState(ID3D11RasterizerState* pRSstate);
		void SetDepthStencilState(ID3D11DepthStencilState* pDSstate, UINT ref = 0x00);

		void SetViewPort(UINT x, UINT y, UINT width, UINT height);

	};

} // namespace

#endif