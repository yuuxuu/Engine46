/**
 * @file CDX11DeviceContext.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/22
 */

#include "CDX11DeviceContext.h"

namespace Engine46 {

	//コンストラクタ
	CDX11DeviceContext::CDX11DeviceContext(ComPtr<ID3D11DeviceContext>& pDeviceContext) :
		m_pDeviceContext(pDeviceContext)
	{}

	// デストラクタ
	CDX11DeviceContext::~CDX11DeviceContext()
	{}

	// バッファを設定
	void CDX11DeviceContext::SetBuffer(ID3D11Buffer* pVertexBuf, ID3D11Buffer* pIndexBuf, UINT strides, UINT offset) {

		m_pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuf, &strides, &offset);

		m_pDeviceContext->IASetIndexBuffer(pIndexBuf, DXGI_FORMAT_R32_UINT, 0);
	}

	// インデックス描画
	void CDX11DeviceContext::DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY topology, UINT numIndexes) {

		m_pDeviceContext->IASetPrimitiveTopology(topology);

		m_pDeviceContext->DrawIndexed(numIndexes, 0, 0);
	}

	// レンダーターゲットビューをクリア
	void CDX11DeviceContext::ClearRenderTargetView(ID3D11RenderTargetView* pRtv) {
		const float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };

		m_pDeviceContext->ClearRenderTargetView(pRtv, color);
	}

	// デプスステンシルビューをクリア
	void CDX11DeviceContext::ClearDespthStencilView(ID3D11DepthStencilView* pDsv, UINT clearFlags) {
		m_pDeviceContext->ClearDepthStencilView(pDsv, clearFlags, 1.0f, 0);
	}

	// レンダーターゲットビューを設定
	void CDX11DeviceContext::SetRenderTargetView(ID3D11RenderTargetView* pRtv, ID3D11DepthStencilView* pDsv) {
		if (pRtv) {
			m_pDeviceContext->OMSetRenderTargets(1, &pRtv, pDsv);
		}
		else {
			m_pDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		}
	}

	// レンダーターゲットビューを複数設定
	void CDX11DeviceContext::SetRenderTargetViews(std::vector<ID3D11RenderTargetView*>& vecRtv, ID3D11DepthStencilView* pDsv) {
		m_pDeviceContext->OMSetRenderTargets(vecRtv.size(), &vecRtv[0], pDsv);
	}

	// バーテックスシェーダーを設定
	void CDX11DeviceContext::SetVsShader(ID3D11VertexShader* pVs) {
		m_pDeviceContext->VSSetShader(pVs, nullptr, 0);
	}

	// ピクセルシェーダーを設定
	void CDX11DeviceContext::SetPsShader(ID3D11PixelShader* pPs) {
		m_pDeviceContext->PSSetShader(pPs, nullptr, 0);
	}

	// ハルシェーダーを設定
	void CDX11DeviceContext::SetHsShader(ID3D11HullShader* pHs) {
		m_pDeviceContext->HSSetShader(pHs, nullptr, 0);
	}

	// ドメインシェーダーを設定
	void CDX11DeviceContext::SetDsShader(ID3D11DomainShader* pDs) {
		m_pDeviceContext->DSSetShader(pDs, nullptr, 0);
	}

	// ジオメトリーシェーダーを設定
	void CDX11DeviceContext::SetGsShader(ID3D11GeometryShader* pGs) {
		m_pDeviceContext->GSSetShader(pGs, nullptr, 0);
	}

	// コンピュートシェーダーを設定
	void CDX11DeviceContext::SetCsShader(ID3D11ComputeShader* pCs) {
		m_pDeviceContext->CSSetShader(pCs, nullptr, 0);
	}

	// ピクセルシェーダーにリソースビューを設定
	void CDX11DeviceContext::SetPSShaderResources(UINT slot, UINT num, ID3D11ShaderResourceView* pSrv) {
		m_pDeviceContext->PSSetShaderResources(slot, num, &pSrv);
	}

	// バーテックスシェーダーにコンスタントバッファを設定
	void CDX11DeviceContext::SetVSConstantBuffers(UINT slot, UINT num, ID3D11Buffer* pBuf) {
		m_pDeviceContext->VSSetConstantBuffers(slot, num, &pBuf);
	}

	// ピクセルシェーダーにコンスタントバッファを設定
	void CDX11DeviceContext::SetPSConstantBuffers(UINT slot, UINT num, ID3D11Buffer* pBuf) {
		m_pDeviceContext->PSSetConstantBuffers(slot, num, &pBuf);
	}

	// サブリソースの更新
	void CDX11DeviceContext::UpdateSubResource(ID3D11Buffer* pBuf, void* pSrcData) {
		m_pDeviceContext->UpdateSubresource(pBuf, 0, nullptr, pSrcData, 0, 0);
	}

	// インプットレイアウトを設定
	void CDX11DeviceContext::SetInputLayout(ID3D11InputLayout* pILayout) {
		m_pDeviceContext->IASetInputLayout(pILayout);
	}

	// サンプラーステートを設定
	void CDX11DeviceContext::SetSamplerState(ID3D11SamplerState* pSstate, UINT slot) {
		m_pDeviceContext->PSSetSamplers(slot, 1, &pSstate);
	}

	// ブレンドステートを設定
	void CDX11DeviceContext::SetBlendState(ID3D11BlendState* pBState) {
		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_pDeviceContext->OMSetBlendState(pBState, blendFactor, 0xffffffff);
	}

	// ラスタライズステートを設定
	void CDX11DeviceContext::SetRasterizerState(ID3D11RasterizerState* pRSstate) {
		m_pDeviceContext->RSSetState(pRSstate);
	}

	// デプスステンシルステートを設定
	void CDX11DeviceContext::SetDepthStencilState(ID3D11DepthStencilState* pDSstate, UINT ref) {
		m_pDeviceContext->OMSetDepthStencilState(pDSstate, ref);
	}

	// ビューポートを設定
	void CDX11DeviceContext::SetViewPort(UINT x, UINT y, UINT width, UINT height) {
		D3D11_VIEWPORT vp = {};
		vp.Width = (float)width;
		vp.Height = (float)height;
		vp.TopLeftX = (float)x;
		vp.TopLeftY = (float)y;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		m_pDeviceContext->RSSetViewports(1, &vp);
	}

} // namespace