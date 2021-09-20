/**
 * @file CDX12Renderer.height
 * @brief
 * @author 木村優
 * @date 2021/08/19
 */

#include "CDX12Renderer.h"
#include "CDX12Device.h"
#include "CDX12Command.h"
#include "CDX12ConstantBuffer.h"
#include "CDX12Mesh.h"
#include "CDX12Texture.h"
#include "CDX12ShaderPackage.h"
#include "CDX12ForwardRendering.h"
#include "CDX12DepthRendering.h"
#include "CDX12DeferredRenderig.h"

#include "../Engine46/CFileSystem.h"
#include "../Engine46/CLight.h"
#include "../Engine46/CCamera.h"

namespace Engine46 {

	constexpr UINT DESCRIPTORHEAP_MAX = STATIC_MAX + 100;

	// コンストラクタ
	CDX12Renderer::CDX12Renderer() :
		m_descriptorHeapOffsetIndex(0)
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

			m_pDX12Device->CreateDescriptorHeap(m_pRtvDescriptorHeap, dhDesc);

			m_rtvHandle[0] = m_pRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			UINT size = m_pDX12Device->GetDescriptorHandleIncrementSize(dhDesc.Type);
			m_rtvHandle[1].ptr = m_rtvHandle[0].ptr + size;

			m_pDX12Device->GetBackBuffer(m_pRtvResource[0], 0);
			m_pDX12Device->CreateRenderTargetView(m_pRtvResource[0].Get(), nullptr, m_rtvHandle[0]);
			
			m_pDX12Device->GetBackBuffer(m_pRtvResource[1], 1);
			m_pDX12Device->CreateRenderTargetView(m_pRtvResource[1].Get(), nullptr, m_rtvHandle[1]);
		}

		{
			D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};

			dhDesc.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dhDesc.NumDescriptors	= 1;
			dhDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

			m_pDX12Device->CreateDescriptorHeap(m_pDsvDescriptorHeap, dhDesc);

			m_dsvHandle = m_pDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

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

			CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

			m_pDX12Device->CreateResource(m_pDsvResource, prop, rDesc, &clearValue);

			D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
			
			dsvDesc.ViewDimension				= D3D12_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Format						= DXGI_FORMAT_D24_UNORM_S8_UINT;
			dsvDesc.Texture2D.MipSlice			= 0;
			dsvDesc.Flags						= D3D12_DSV_FLAG_NONE;

			m_pDX12Device->CreateDepthStencilView(m_pDsvResource.Get(), &dsvDesc, m_dsvHandle);
		}

		{
			D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};

			dhDesc.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			dhDesc.NumDescriptors	= DESCRIPTORHEAP_MAX;
			dhDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

			m_pDX12Device->CreateDescriptorHeap(m_pCbDescriptorHeap, dhDesc);

			this->CreateConstantBuffer(m_pCameraCB, sizeof(CameraCB));
			this->CreateConstantBuffer(m_pDirectionalLightCB, sizeof(DirectionalLightCB));
			this->CreateConstantBuffer(m_pPointLightCB, sizeof(PointLightCB));
			this->CreateConstantBuffer(m_pSpotLightCB, sizeof(SpotLightCB));
		}

		//m_pForwardRendering = std::make_unique<CDX12ForwardRendering>(m_pDX12Device.get(), m_pDX12Command.get());
		//if (!m_pForwardRendering->Initialize(width, height)) return false;
		
		m_pDeferredRendering = std::make_unique<CDX12DeferredRenderig>(m_pDX12Device.get(), m_pDX12Command.get());
		if (!m_pDeferredRendering->Initialize(width, height)) return false;

		m_pDepthRendring = std::make_unique<CDX12DepthRendering>(m_pDX12Device.get(), m_pDX12Command.get());
		if (!m_pDepthRendring->Initialize(width, height)) return false;

		m_pDX12Command->CloseCommandList();

		m_windowRect = RECT(width, height);

		return true;
	}

	// 終了
	void CDX12Renderer::Finalize() {

	}

	// 描画準備開始
	void CDX12Renderer::Begine(CSceneBase* pScene) {
		if (!m_pRenderSprite) {
			m_pRenderSprite = std::make_unique<CSprite>("RenderSprite");
			m_pRenderSprite->SetMesh("RenderSpriteMesh");
			m_pRenderSprite->SetMaterial("RenderSpriteMaterial");
			m_pRenderSprite->SetShaderPackage("Sprite.hlsl");
			m_pRenderSprite->InitializeResource(this);
		}

		if (pScene) {
			CCamera* pCamera = pScene->GetCameraFromScene();
			if (pCamera) {

				Matrix matVP = pCamera->GetViewProjectionMatrix();
				matVP.dx_m = DirectX::XMMatrixTranspose(matVP.dx_m);

				CameraCB cb = {
					matVP,
					pCamera->GetPos(),
				};

				m_pCameraCB->Update(&cb);
			}

			std::vector<CLight*> pLights = pScene->GetLightsFromScene();
			if (!pLights.empty()) {

				DirectionalLightCB directionalLightCb = {};
				PointLightCB pointLightCb = {};
				SpotLightCB spotLightCb = {};

				for (const auto light : pLights)
				{
					switch (light->GetLightType())
					{
					case LightType::Directional:
						directionalLightCb.pos = light->GetPos();
						directionalLightCb.diffuse = light->GetLightDiffuse();
						directionalLightCb.specular = light->GetLightSpecular();
						break;
					case LightType::Point:
						pointLightCb.pointLights[pointLightCb.numPointLight].pos = light->GetPos();
						pointLightCb.pointLights[pointLightCb.numPointLight].diffuse = light->GetLightDiffuse();
						pointLightCb.pointLights[pointLightCb.numPointLight].specular = light->GetLightSpecular();
						pointLightCb.pointLights[pointLightCb.numPointLight].attenuation = light->GetLightAttenuation();

						pointLightCb.numPointLight++;
						break;
					case LightType::Spot:
						spotLightCb.spotLights[spotLightCb.numSpotLight].pos = light->GetPos();
						spotLightCb.spotLights[spotLightCb.numSpotLight].diffuse = light->GetLightDiffuse();
						spotLightCb.spotLights[spotLightCb.numSpotLight].specular = light->GetLightSpecular();
						spotLightCb.spotLights[spotLightCb.numSpotLight].attenuation = light->GetLightAttenuation();

						spotLightCb.numSpotLight++;
						break;
					}
				}

				m_pDirectionalLightCB->Update(&directionalLightCb);

				if (pointLightCb.numPointLight > 0) {
					m_pPointLightCB->Update(&pointLightCb);
				}
				if (spotLightCb.numSpotLight > 0) {
					m_pSpotLightCB->Update(&spotLightCb);
				}
			}
		}
	}

	// 描画
	bool CDX12Renderer::Render(CSceneBase* pScene) {

		m_pDX12Command->ResetCommandList();

		m_pDX12Command->SetRect(m_windowRect.w, m_windowRect.h);
		m_pDX12Command->SetViewPort(0, 0, m_windowRect.w, m_windowRect.h);

		if (m_pDepthRendring) {
			m_pDepthRendring->Rendering(pScene);
			Reset();
		}

		if (m_pDeferredRendering) {
			m_pDeferredRendering->Rendering(pScene);
			Reset();
		}

		UINT index = m_pDX12Device->GetCurrentBackBufferIndex();

		m_pDX12Command->SetResourceBarrier(m_pRtvResource[index].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

		m_pDX12Command->ClearRenderTargetView(m_rtvHandle[index]);
		m_pDX12Command->ClearDepthStencilView(m_dsvHandle, D3D12_CLEAR_FLAG_DEPTH);
		m_pDX12Command->SetRenderTargetView(&m_rtvHandle[index], &m_dsvHandle);

		UINT width = (UINT)m_windowRect.w / (RENDER_TARGET_SIZE + 1);
		UINT height = m_windowRect.h / (RENDER_TARGET_SIZE + 1);
		UINT x = 0;
		UINT y = (UINT)m_windowRect.h - height;

		if (m_pDeferredRendering) {
			m_pDeferredRendering->DrawForSceneLighting(m_pRenderSprite.get());

			m_pDeferredRendering->DrawForRenderScene(m_pRenderSprite.get(), x, y, width, height);
		}

		if (m_pDepthRendring) {
			x += width * RENDER_TARGET_SIZE;
			m_pDepthRendring->DrawForRenderScene(m_pRenderSprite.get(), x, y, width, height);
		}

		m_pDX12Command->SetResourceBarrier(m_pRtvResource[index].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);

		m_pDX12Command->ExecuteComandLists();

		return m_pDX12Device->Present();
	}

	// コマンドを発行しリセット
	void CDX12Renderer::Reset() {

		m_pDX12Command->ExecuteComandLists();

		m_pDX12Command->ResetCommandList();

		m_pDX12Command->SetRect(m_windowRect.w, m_windowRect.h);
		m_pDX12Command->SetViewPort(0, 0, m_windowRect.w, m_windowRect.h);
	}

	void CDX12Renderer::SetConstantBuffers() {
		m_pCameraCB->Set((UINT)MyRootSignature_01::CBV_CAMERA);
		m_pDirectionalLightCB->Set((UINT)MyRootSignature_01::CBV_DirectionalLight);
		m_pPointLightCB->Set((UINT)MyRootSignature_01::CBV_PointLight);
		m_pSpotLightCB->Set((UINT)MyRootSignature_01::CBV_SpotLight);
	}

	// コンスタントバッファ作成
	void CDX12Renderer::CreateConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer, UINT byteWidth) {
		pConstantBuffer = std::make_unique<CDX12ConstantBuffer>(m_pDX12Device.get(), m_pDX12Command.get());

		pConstantBuffer->CreateConstantBuffer(byteWidth);

		if (m_descriptorHeapOffsetIndex <= DESCRIPTORHEAP_MAX) {
			dynamic_cast<CDX12ConstantBuffer*>(pConstantBuffer.get())->CreateConstantBufferView(m_pCbDescriptorHeap.Get(), m_descriptorHeapOffsetIndex++);
		}

	}

	// メッシュ作成
	void CDX12Renderer::CreateMesh(std::unique_ptr<CMeshBase>& pMesh, const char* meshName) {
		pMesh = std::make_unique<CDX12Mesh>(m_pDX12Device.get(), m_pDX12Command.get(), meshName);
	}

	// テクスチャ作成
	void CDX12Renderer::CreateTexture(std::unique_ptr<CTextureBase>& pTexture, const char* textureName) {
		FileInfo* fileInfo = CFileSystem::GetFileSystem().GetFileInfoFromMap(textureName);

		if (!fileInfo) return;

		pTexture = std::make_unique<CDX12Texture>(m_pDX12Device.get(), m_pDX12Command.get(), textureName);

		if (pTexture->LoadTexture(fileInfo->filePath.c_str())) {
			pTexture->CreateTexture();

			dynamic_cast<CDX12Texture*>(pTexture.get())->CreateShaderResourceView(m_pCbDescriptorHeap.Get(), m_descriptorHeapOffsetIndex++);
		}
	}

	// シェーダー作成
	void CDX12Renderer::CreateShader(std::unique_ptr<CShaderPackage>& pShaderPackage, const char* shaderName) {
		FileInfo* fileInfo = CFileSystem::GetFileSystem().GetFileInfoFromMap(shaderName);

		if (!fileInfo) return;

		pShaderPackage = std::make_unique<CDX12ShaderPackage>(m_pDX12Device.get(), m_pDX12Command.get(), shaderName);

		for (const auto& info : SHADER_INFOS) {
			ComPtr<ID3DBlob> pBlob;

			if (pShaderPackage->CompileShader(pBlob, fileInfo->filePath.c_str(), info.entryPoint, info.shaderModel)) {
				std::unique_ptr<CShaderBase> shader = std::make_unique<CShaderBase>(shaderName, pBlob, info.shadeType);

				pShaderPackage->AddShaderToVec(shader);
			}
		}

		if (pShaderPackage->IsCompile()) {
			dynamic_cast<CDX12ShaderPackage*>(pShaderPackage.get())->SetDescriptorHeap(m_pCbDescriptorHeap.Get());

			D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
			dynamic_cast<CDX12ShaderPackage*>(pShaderPackage.get())->InitializeGraphics(gpsDesc);

			D3D12_COMPUTE_PIPELINE_STATE_DESC cpsDesc = {};
			dynamic_cast<CDX12ShaderPackage*>(pShaderPackage.get())->InitializeCompute(cpsDesc);
		}
	}

	// レンダーテクスチャ作成
	void CDX12Renderer::CreateRenderTexture(std::unique_ptr<CDX12Texture>& pDX12RenderTexture, D3D12_RESOURCE_DESC& rDesc, D3D12_CLEAR_VALUE& clearValue, TextureType type) {
		pDX12RenderTexture = std::make_unique<CDX12Texture>(m_pDX12Device.get(), m_pDX12Command.get());
		
		switch (type) {
		case TextureType::Render:
			pDX12RenderTexture->CreateTexture(rDesc, clearValue);
			break;
		case TextureType::Depth:
			pDX12RenderTexture->CreateDepthTexture(rDesc, clearValue);
			break;
		case TextureType::Stencil:
			pDX12RenderTexture->CreateStencilTexture(rDesc, clearValue);
			break;
		}

		pDX12RenderTexture.get()->CreateShaderResourceView(m_pCbDescriptorHeap.Get(), m_descriptorHeapOffsetIndex++);
	}

} // namespace
