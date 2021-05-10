/**
 * @file CForwardRendering.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/04
 */

#include "CForwardRendering.h"

namespace Engine46 {

	// コンストラクタ
	CDX11ForwardRendering::CDX11ForwardRendering(CDX11Renderer* pRenderer) :
		pRenderer(pRenderer)
	{}

	// デストラクタ
	CDX11ForwardRendering::~CDX11ForwardRendering()
	{}

	// 初期化
	bool CDX11ForwardRendering::Initialize(UINT width, UINT height) {

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

			if (!pRenderer->CreateTexture2D(m_pTex2D, texDesc)) return false;

			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.ViewDimension				= D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Format						= texDesc.Format;
			rtvDesc.Texture2DArray.ArraySize	= texDesc.ArraySize;

			if (!pRenderer->CreateRenderTargetView(m_pRtv, m_pTex2D.Get(), rtvDesc)) return false;

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.ViewDimension		= D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Format				= srvDesc.Format;
			srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

			if (!pRenderer->CreateShaderResourceView(m_pSrv, m_pTex2D.Get(), srvDesc)) return false;
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

			pRenderer->SetSamplerState(sDesc, 0);
		}

		{
			D3D11_BLEND_DESC bDesc = {};
			bDesc.AlphaToCoverageEnable		= FALSE;
			bDesc.IndependentBlendEnable	= FALSE;

			for (int i = 0; i < 8; ++i) {
				bDesc.RenderTarget[i].BlendEnable			= TRUE;
				bDesc.RenderTarget[i].SrcBlend				= D3D11_BLEND_ONE;
				bDesc.RenderTarget[i].DestBlend				= D3D11_BLEND_ONE;
				bDesc.RenderTarget[i].BlendOp				= D3D11_BLEND_OP_ADD;
				bDesc.RenderTarget[i].SrcBlendAlpha			= D3D11_BLEND_ONE;
				bDesc.RenderTarget[i].DestBlendAlpha		= D3D11_BLEND_ONE;
				bDesc.RenderTarget[i].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
				bDesc.RenderTarget[i].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
			}

			pRenderer->SetBlendState(bDesc);
		}

		{
			D3D11_RASTERIZER_DESC rDesc = {};
			rDesc.FillMode = D3D11_FILL_SOLID;
			rDesc.CullMode = D3D11_CULL_NONE;
			
			pRenderer->SetRasterizerState(rDesc);
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

			pRenderer->SetDepthStencilState(dsDesc, 0x00);
		}

		pRenderer->SetViewPort(width, height);

		return true;
	}

	// レンダリング開始
	void CDX11ForwardRendering::Begine() {

		pRenderer->ClearRenderTargetView(m_pRtv.Get());

		pRenderer->SetRenderTargetView(m_pRtv.Get(), nullptr);
	}

	// レンダリング終了
	void CDX11ForwardRendering::End() {

	}

} // namespace