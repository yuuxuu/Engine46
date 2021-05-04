/**
 * @file CDX11Renderer.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/02
 */

#include "CDX11Renderer.h"
#include "CForwardRendering.h"

namespace Engine46 {

	// コンストラクタ
	CDX11Renderer::CDX11Renderer() :
		m_iLayoutBufSize(0)
	{}

	// デストラクタ
	CDX11Renderer::~CDX11Renderer() 
	{}

	// 初期化
	bool CDX11Renderer::Initialize(HWND hwnd, UINT width, UINT height) {
		
		UINT flag = 0;
#ifdef _DEBUG
		flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		// スワップチェーンの設定
		DXGI_SWAP_CHAIN_DESC scDesc = {};
		scDesc.BufferDesc.Width						= width;
		scDesc.BufferDesc.Height					= height;
		scDesc.BufferDesc.RefreshRate.Numerator		= 60;
		scDesc.BufferDesc.RefreshRate.Denominator	= 1;
		scDesc.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
		scDesc.SampleDesc.Count						= 1;
		scDesc.SampleDesc.Quality					= 0;
		scDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scDesc.BufferCount							= 1;
		scDesc.OutputWindow							= hwnd;
		scDesc.Windowed								= TRUE;
		scDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
		scDesc.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_10_1,
		};
		UINT numFeatureLevel = sizeof(featureLevels) / sizeof(featureLevels[0]);
		D3D_FEATURE_LEVEL featureLevel;

		// デバイスとスワップチェーン作成
		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			flag,
			featureLevels,
			numFeatureLevel,
			D3D11_SDK_VERSION,
			&scDesc,
			&m_pSwapChain,
			&m_pDevice,
			&featureLevel,
			&m_pDeviceContext
		);
		if (FAILED(hr)) {
			MessageBox(NULL, "DirectX11初期化：失敗", "MessageBox", MB_OK);
			return false;
		}

		ID3D11Texture2D* pTex2D;
		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pTex2D);
		if (FAILED(hr)) {
			MessageBox(NULL, "バックバッファ取得：失敗", "MessageBox", MB_OK);
			return false;
		}

		D3D11_TEXTURE2D_DESC texDesc;
		pTex2D->GetDesc(&texDesc);

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.ViewDimension				= D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Format						= texDesc.Format;
		rtvDesc.Texture2DArray.ArraySize	= texDesc.ArraySize;

		if(!this->CreateRenderTargetView(m_pRtv, pTex2D, rtvDesc)) return false;

		m_pDX11FRendering = std::make_unique<CDX11ForwardRendering>(this);
		if (!m_pDX11FRendering->Initialize(width, height)) return false;

		return true;
	}

	// 終了
	void CDX11Renderer::Finalize() {

	}

	// 描画
	bool CDX11Renderer::Render() {

		m_pDX11FRendering->Begine();

		m_pDX11FRendering->End();

		this->ClearRenderTargetView(m_pRtv.Get());
		this->SetRenderTargetView(m_pRtv.Get(), nullptr);

		HRESULT hr = m_pSwapChain->Present(0, 0);
		if (FAILED(hr)) {
			return false;
		}

		return true;
	}

	// テクスチャ2D作成
	bool CDX11Renderer::CreateTexture2D(
		ComPtr<ID3D11Texture2D>& pTex2D,
		D3D11_TEXTURE2D_DESC& texDesc,
		D3D11_SUBRESOURCE_DATA* pInitData) {

		HRESULT hr = m_pDevice->CreateTexture2D(&texDesc, pInitData, &pTex2D);
		if (FAILED(hr)) {
			MessageBox(NULL, "Texture2D作成：失敗", "MessageBox", MB_OK);
			return false;
		}

		return true;
	}

	// レンダーターゲットビュー作成
	bool CDX11Renderer::CreateRenderTargetView(
		ComPtr<ID3D11RenderTargetView>& pRtv,
		ID3D11Texture2D* pTex2D,
		D3D11_RENDER_TARGET_VIEW_DESC& rtvDesc) {

		HRESULT hr = m_pDevice->CreateRenderTargetView(pTex2D, &rtvDesc, &pRtv);
		if (FAILED(hr)) {
			MessageBox(NULL, "レンダーターゲットビュー作成：失敗", "MessageBox", MB_OK);
			return false;
		}

		return true;
	}

	// シェーダーリソースビュー作成
	bool CDX11Renderer::CreateShaderResourceView(
		ComPtr<ID3D11ShaderResourceView>& pSrv,
		ID3D11Texture2D* pTex2D,
		D3D11_SHADER_RESOURCE_VIEW_DESC& srvDesc) {

		HRESULT hr = m_pDevice->CreateShaderResourceView(pTex2D, &srvDesc, &pSrv);
		if (FAILED(hr)) {
			MessageBox(NULL, "シェーダーリソースビュー作成：失敗", "MessageBox", MB_OK);
			return false;
		}

		return true;
	}

	// デプスステンシルビュー作成
	bool CDX11Renderer::CreateDepthStencilView(
		ComPtr<ID3D11DepthStencilView>& pDsv,
		ID3D11Texture2D* pTex2D,
		D3D11_DEPTH_STENCIL_VIEW_DESC& dsvDesc) {

		HRESULT hr = m_pDevice->CreateDepthStencilView(pTex2D, &dsvDesc, &pDsv);
		if (FAILED(hr)) {
			MessageBox(NULL, "デプスステンシルビュー作成：失敗", "MessageBox", MB_OK);
			return false;
		}

		return true;
	}

	// レンダーターゲットビューをクリア
	void CDX11Renderer::ClearRenderTargetView(ID3D11RenderTargetView* pRtv) {
		const float color[] = { 0.5f, 0.5f, 0.0f, 1.0f };

		m_pDeviceContext->ClearRenderTargetView(pRtv, color);
	}

	// デプスステンシルビューをクリア
	void CDX11Renderer::ClearDespthStencilView(ID3D11DepthStencilView* pDsv, UINT clearFlags) {
		m_pDeviceContext->ClearDepthStencilView(pDsv, clearFlags, 1.0f, 0);
	}

	// レンダーターゲットビューを設定
	void CDX11Renderer::SetRenderTargetView(ID3D11RenderTargetView* pRtv, ID3D11DepthStencilView* pDsv) {
		m_pDeviceContext->OMSetRenderTargets(1, &pRtv, pDsv);
	}

	// レンダーターゲットビューを複数設定
	void CDX11Renderer::SetRenderTargetViews(std::vector<ComPtr<ID3D11RenderTargetView>>& vecRtv, ID3D11DepthStencilView* pDsv) {
		m_pDeviceContext->OMSetRenderTargets(vecRtv.size(), &vecRtv[0], pDsv);
	}

	// インプットレイアウトを設定
	void CDX11Renderer::SetInputLayout(void* pBuf, size_t bufSize) {

		if (m_iLayoutBufSize < bufSize) {
			m_iLayoutBufSize = bufSize;

			D3D11_INPUT_ELEMENT_DESC ieDesc[] = {
				{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT   , 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT      , 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BINORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BONEINDEX",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BONEWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 88, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			ID3D11InputLayout* pILayout;

			HRESULT hr = m_pDevice->CreateInputLayout(ieDesc, ARRAYSIZE(ieDesc), pBuf, bufSize, &pILayout);
			if (FAILED(hr)) {
				MessageBox(NULL, "インプットレイアウト作成：失敗", "MessageBox", MB_OK);
			}

			m_pDeviceContext->IASetInputLayout(pILayout);
		}
	}

	// サンプラーステートを設定
	void CDX11Renderer::SetSamplerState(D3D11_SAMPLER_DESC& sDesc, UINT slot) {

		ID3D11SamplerState* pSstate;

		HRESULT hr = m_pDevice->CreateSamplerState(&sDesc, &pSstate);
		if (FAILED(hr)) {
			MessageBox(NULL, "サンプラーステート作成：失敗", "MessageBox", MB_OK);
		}

		m_pDeviceContext->PSSetSamplers(slot, 1, &pSstate);
	}

	// ブレンドステートを設定
	void CDX11Renderer::SetBlendState(D3D11_BLEND_DESC& bDesc) {

		ID3D11BlendState* pBState;

		HRESULT hr = m_pDevice->CreateBlendState(&bDesc, &pBState);
		if (FAILED(hr)) {
			MessageBox(NULL, "ブレンドステート作成：失敗", "MessageBox", MB_OK);
		}

		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_pDeviceContext->OMSetBlendState(pBState, blendFactor, 0xffffffff);
	}

	// ラスタライズステートを設定
	void CDX11Renderer::SetRasterizerState(D3D11_RASTERIZER_DESC& rsDesc) {

		ID3D11RasterizerState* pRSstate;

		HRESULT hr = m_pDevice->CreateRasterizerState(&rsDesc, &pRSstate);
		if (FAILED(hr)) {
			MessageBox(NULL, "ラスタライザステート作成：失敗", "MessageBox", MB_OK);
		}

		m_pDeviceContext->RSSetState(pRSstate);
	}

	// デプスステンシルステートを設定
	void CDX11Renderer::SetDepthStencilState(D3D11_DEPTH_STENCIL_DESC& dsDesc, UINT ref) {
		
		ID3D11DepthStencilState* pDSstate;

		HRESULT hr = m_pDevice->CreateDepthStencilState(&dsDesc, &pDSstate);
		if (FAILED(hr)) {
			MessageBox(NULL, "デプスステンシルステート作成：失敗", "MessageBox", MB_OK);
		}

		m_pDeviceContext->OMSetDepthStencilState(pDSstate, ref);
	}

	// ビューポートを設定
	void CDX11Renderer::SetViewPort(int width, int height) {
		D3D11_VIEWPORT vp = {};
		vp.Width	= (float)width;
		vp.Height	= (float)height;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		m_pDeviceContext->RSSetViewports(1, &vp);
	}

} // namespace