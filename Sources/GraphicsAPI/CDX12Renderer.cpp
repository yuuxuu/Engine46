/**
 * @file CDX12Renderer.h
 * @brief
 * @author 木村優
 * @date 2021/08/19
 */

#include "CDX12Renderer.h"
#include "CDX12Device.h"
#include "CDX12Command.h"

namespace Engine46 {

	// コンストラクタ
	CDX12Renderer::CDX12Renderer()
	{}

	// デストラクタ
	CDX12Renderer::~CDX12Renderer()
	{}

	// 初期化
	bool CDX12Renderer::Initialize(HWND hwnd, UINT width, UINT height) {

		ComPtr<ID3D12CommandQueue> pCommandQueue;

		m_pDX12Device = std::make_unique<CDX12Device>();
		if (!m_pDX12Device->Initialize(pCommandQueue, hwnd, width, height)) return false;

		m_pDX12Command = std::make_unique<CDX12Command>(pCommandQueue);
		if (!m_pDX12Command->Initialize(m_pDX12Device.get())) return false;

		{
			D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};
			
			dhDesc.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			dhDesc.NumDescriptors	= 2;
			dhDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

			m_pDX12Device->CreateDescriptorHeap(m_pD3D12RtvDescriptorHeap, dhDesc);

			//D3D12_RESOURCE_DESC rDesc = {};
			//
			//rDesc.Width				= width;
			//rDesc.Height			= height;
			//rDesc.MipLevels			= 1;
			//rDesc.Format			= DXGI_FORMAT_UNKNOWN;
			//rDesc.Flags				= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
			//rDesc.DepthOrArraySize	= 1;
			//rDesc.SampleDesc		= { 1 , 0 };
			//rDesc.Dimension			= D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			//rDesc.Layout			= D3D12_TEXTURE_LAYOUT_UNKNOWN;
			//
			//D3D12_CLEAR_VALUE clearValue = {};
			//
			//clearValue.Format	= rDesc.Format;
			//clearValue.Color[0]	= 0.0f;
			//clearValue.Color[1]	= 0.0f;
			//clearValue.Color[2]	= 0.0f;
			//clearValue.Color[3]	= 1.0f;

			m_rtvHandle[0] = m_pD3D12RtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			UINT size = m_pDX12Device->GetDescriptorHandleIncrementSize(dhDesc.Type);
			m_rtvHandle[1].ptr = m_rtvHandle[0].ptr + size;

			m_pDX12Device->CreateRenderTargetView(m_pRtvResource[0], nullptr, m_rtvHandle[0], 0);
			m_pDX12Device->CreateRenderTargetView(m_pRtvResource[1], nullptr, m_rtvHandle[1], 1);
		}

		{
			D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};

			dhDesc.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dhDesc.NumDescriptors	= 1;
			dhDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

			m_pDX12Device->CreateDescriptorHeap(m_pD3D12DsvDescriptorHeap, dhDesc);

			m_dsvHandle = m_pD3D12DsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

			D3D12_RESOURCE_DESC rDesc = {};

			rDesc.Width				= width;
			rDesc.Height			= height;
			rDesc.MipLevels			= 1;
			rDesc.Format			= DXGI_FORMAT_R24G8_TYPELESS;
			rDesc.Flags				= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			rDesc.DepthOrArraySize	= 1;
			rDesc.SampleDesc		= { 1 , 0 };
			rDesc.Dimension			= D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			rDesc.Layout			= D3D12_TEXTURE_LAYOUT_UNKNOWN;

			D3D12_CLEAR_VALUE clearValue = {};

			clearValue.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
			clearValue.DepthStencil.Depth	= 1.0f;
			clearValue.DepthStencil.Stencil = 0;

			m_pDX12Device->CreateResource(m_pDsvResource, rDesc, clearValue);

			D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
			
			dsvDesc.ViewDimension				= D3D12_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Format						= DXGI_FORMAT_D24_UNORM_S8_UINT;
			dsvDesc.Texture2D.MipSlice			= 0;
			dsvDesc.Flags						= D3D12_DSV_FLAG_NONE;

			m_pDX12Device->CreateDepthStencilView(m_pDsvResource.Get(), &dsvDesc, m_dsvHandle);
		}

		m_pDX12Command->CloseCommandList();

		m_windowRect = RECT(width, height);

		return true;
	}

	// 終了
	void CDX12Renderer::Finalize() {

	}

	// 描画準備開始
	void CDX12Renderer::Begine(CSceneBase* pScene) {

	}

	// 描画
	bool CDX12Renderer::Render(CSceneBase* pScene) {

		m_pDX12Command->ResetCommandList();

		m_pDX12Command->SetRect(m_windowRect.w, m_windowRect.h);
		m_pDX12Command->SetViewPort(m_windowRect.w, m_windowRect.h);

		UINT index = m_pDX12Device->GetCurrentBackBufferIndex();

		m_pDX12Command->SetResourceBarrier(m_pRtvResource[index].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

		m_pDX12Command->ClearRenderTargetView(m_rtvHandle[index]);
		m_pDX12Command->ClearDepthStencilView(m_dsvHandle, D3D12_CLEAR_FLAG_DEPTH);
		m_pDX12Command->SetRenderTargetView(m_rtvHandle[index], m_dsvHandle);

		//pScene->Draw();

		m_pDX12Command->SetResourceBarrier(m_pRtvResource[index].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);

		m_pDX12Command->ExecuteComandLists();

		return m_pDX12Device->Present();
	}

	// コンスタントバッファ作成
	void CDX12Renderer::CreateConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer) {

	}

	// メッシュ作成
	void CDX12Renderer::CreateMesh(std::unique_ptr<CMeshBase>& pMesh, const char* meshName) {

	}

	// テクスチャ作成
	void CDX12Renderer::CreateTexture(std::unique_ptr<CTextureBase>& pTexture, const char* textureName) {

	}

	// シェーダー作成
	void CDX12Renderer::CreateShader(std::unique_ptr<CShaderPackage>& pShaderPackage, const char* shaderName) {

	}

} // namespace
