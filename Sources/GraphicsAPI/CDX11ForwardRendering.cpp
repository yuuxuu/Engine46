/**
 * @file CDX11ForwardRendering.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/04
 */

#include "CDX11ForwardRendering.h"
#include "CDX11Device.h"
#include "CDX11DeviceContext.h"
#include "CDX11Texture.h"
#include "CDX11Renderer.h"

#include "../Engine46/CRendererSystem.h"

namespace Engine46 {

	// コンストラクタ
	CDX11ForwardRendering::CDX11ForwardRendering(CDX11Device* pDX11Device, CDX11DeviceContext* pDX11DeviceContext) :
		pDX11Device(pDX11Device),
		pDX11DeviceContext(pDX11DeviceContext)
	{}

	// デストラクタ
	CDX11ForwardRendering::~CDX11ForwardRendering()
	{}

	// 初期化
	bool CDX11ForwardRendering::Initialize(UINT width, UINT height) {

		CDX11Renderer* pRenderer = dynamic_cast<CDX11Renderer*>(CRendererSystem::GetRendererSystem().GetRenderer());

		{
			D3D11_TEXTURE2D_DESC texDesc = {};

			texDesc.Width				= width;
			texDesc.Height				= height;
			texDesc.MipLevels			= 1;
			texDesc.ArraySize			= 1;
			texDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
			texDesc.SampleDesc.Count	= 1;
			texDesc.SampleDesc.Quality	= 0;
			texDesc.Usage				= D3D11_USAGE_DEFAULT;
			texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			texDesc.CPUAccessFlags		= 0;
			texDesc.MiscFlags			= 0;

			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};

			rtvDesc.ViewDimension	= D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Format			= texDesc.Format;

			std::unique_ptr<CDX11Texture> pRenderTex;
			if (pRenderer) {
				pRenderer->CreateRenderTexture(pRenderTex, texDesc);

				if (!pDX11Device->CreateRenderTargetView(m_pRtv, pRenderTex->GetTexture2D(), rtvDesc)) return false;

				m_pRenderTex = std::move(pRenderTex);
			}
		}

		{
			D3D11_TEXTURE2D_DESC texDesc = {};

			texDesc.Width				= width;
			texDesc.Height				= height;
			texDesc.MipLevels			= 1;
			texDesc.ArraySize			= 1;
			texDesc.Format				= DXGI_FORMAT_R24G8_TYPELESS;
			texDesc.SampleDesc.Count	= 1;
			texDesc.SampleDesc.Quality	= 0;
			texDesc.Usage				= D3D11_USAGE_DEFAULT;
			texDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
			texDesc.CPUAccessFlags		= 0;
			texDesc.MiscFlags			= 0;

			ComPtr<ID3D11Texture2D> pTex;
			if (!pDX11Device->CreateTexture2D(pTex, texDesc)) return false;

			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

			dsvDesc.ViewDimension				= D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Format						= DXGI_FORMAT_D24_UNORM_S8_UINT;
			dsvDesc.Texture2D.MipSlice			= 0;
			dsvDesc.Texture2DArray.ArraySize	= texDesc.ArraySize;

			if (!pDX11Device->CreateDepthStencilView(m_pDsv, pTex.Get(), dsvDesc)) return false;
		}

		{
			D3D11_SAMPLER_DESC sDesc = {};

			sDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sDesc.AddressU			= D3D11_TEXTURE_ADDRESS_CLAMP;
			sDesc.AddressV			= D3D11_TEXTURE_ADDRESS_CLAMP;
			sDesc.AddressW			= D3D11_TEXTURE_ADDRESS_CLAMP;
			sDesc.BorderColor[0]	= 1.0f;
			sDesc.BorderColor[1]	= 1.0f;
			sDesc.BorderColor[2]	= 1.0f;
			sDesc.BorderColor[3]	= 1.0f;
			sDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;
			sDesc.MaxAnisotropy		= 1;
			sDesc.MipLODBias		= 0;
			sDesc.MinLOD			= -FLT_MAX;
			sDesc.MaxLOD			= +FLT_MAX;

			ComPtr<ID3D11SamplerState> samplerState;
			pDX11Device->CreateSamplerState(samplerState, sDesc);
			pDX11DeviceContext->SetSamplerState(samplerState.Get(), 0);
		}

		{
			D3D11_BLEND_DESC bDesc = {};

			bDesc.AlphaToCoverageEnable		= FALSE;
			bDesc.IndependentBlendEnable	= FALSE;

			for (int i = 0; i < 8; ++i) {
				bDesc.RenderTarget[i].BlendEnable			= TRUE;
				bDesc.RenderTarget[i].SrcBlend				= D3D11_BLEND_ONE;
				bDesc.RenderTarget[i].DestBlend				= D3D11_BLEND_ZERO;
				bDesc.RenderTarget[i].BlendOp				= D3D11_BLEND_OP_ADD;
				bDesc.RenderTarget[i].SrcBlendAlpha			= D3D11_BLEND_ONE;
				bDesc.RenderTarget[i].DestBlendAlpha		= D3D11_BLEND_ZERO;
				bDesc.RenderTarget[i].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
				bDesc.RenderTarget[i].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
			}

			ComPtr<ID3D11BlendState> pBlendState;
			pDX11Device->CreateBlendState(pBlendState, bDesc);
			pDX11DeviceContext->SetBlendState(pBlendState.Get());
		}

		{
			D3D11_RASTERIZER_DESC rsDesc = {};

			rsDesc.FillMode = D3D11_FILL_SOLID;
			rsDesc.CullMode = D3D11_CULL_NONE;
			
			ComPtr<ID3D11RasterizerState> pRSState;
			pDX11Device->CreateRasterizerState(pRSState, rsDesc);
			pDX11DeviceContext->SetRasterizerState(pRSState.Get());
		}

		{
			D3D11_DEPTH_STENCIL_DESC dsDesc = {};

			dsDesc.DepthEnable					= true;
			dsDesc.DepthWriteMask				= D3D11_DEPTH_WRITE_MASK_ALL;
			dsDesc.DepthFunc					= D3D11_COMPARISON_LESS_EQUAL;
			dsDesc.StencilEnable				= false;
			dsDesc.StencilReadMask				= D3D11_DEFAULT_STENCIL_READ_MASK;
			dsDesc.StencilWriteMask				= D3D11_DEFAULT_STENCIL_WRITE_MASK;
			dsDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilFunc		= D3D11_COMPARISON_EQUAL;
			dsDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
			dsDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;
			dsDesc.BackFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
			dsDesc.BackFace.StencilFunc			= D3D11_COMPARISON_EQUAL;

			ComPtr<ID3D11DepthStencilState> pDSState;
			pDX11Device->CreateDepthStencilState(pDSState, dsDesc);
			pDX11DeviceContext->SetDepthStencilState(pDSState.Get(), 0x00);
		}

		pDX11DeviceContext->SetViewPort(0, 0, width, height);

		return true;
	}

	// レンダリング開始
	void CDX11ForwardRendering::Begine() {
		pDX11DeviceContext->ClearRenderTargetView(m_pRtv.Get());
		pDX11DeviceContext->ClearDespthStencilView(m_pDsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL);
		pDX11DeviceContext->SetRenderTargetView(m_pRtv.Get(), m_pDsv.Get());
	}

	// レンダリング終了
	void CDX11ForwardRendering::End() {
		pDX11DeviceContext->SetRenderTargetView(nullptr, nullptr);
	}

	// 描画
	void CDX11ForwardRendering::Rendering(CSceneBase* pScene) {

		Begine();

		pScene->Draw();

		End();
	}

	// 描画したシーンを描画
	void CDX11ForwardRendering::DrawForRenderScene(CSprite* pSprite, UINT x, UINT y, UINT width, UINT height) {

		if (!pSprite) return;

		pDX11DeviceContext->SetViewPort(x, y, width, height);

		pSprite->SetTexture(m_pRenderTex.get());

		pSprite->Draw();
	}

} // namespace