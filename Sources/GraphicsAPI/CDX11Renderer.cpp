﻿/**
 * @file CDX11Renderer.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/02
 */

#include "CDX11Renderer.h"
#include "CDX11Device.h"
#include "CDX11DeviceContext.h"
#include "CDX11ForwardRendering.h"
#include "CDX11ConstantBuffer.h"
#include "CDX11Mesh.h"
#include "CDX11Shader.h"
#include "CDX11Texture.h"

#include "../Engine46/CGameSystem.h"
#include "../Engine46/CFileSystem.h"
#include "../Engine46/CShaderManager.h"
#include "../Engine46/CSprite.h"
#include "../Engine46/CLight.h"
#include "../Engine46/CCamera.h"

namespace Engine46 {

	// コンストラクタ
	CDX11Renderer::CDX11Renderer() :
		m_layoutBufSize(0)
	{}

	// デストラクタ
	CDX11Renderer::~CDX11Renderer() 
	{}

	// 初期化
	bool CDX11Renderer::Initialize(HWND hwnd, UINT width, UINT height) {

		ComPtr<ID3D11DeviceContext> pDeviceContext;

		m_pDX11Device = std::make_unique<CDX11Device>();
		if (!m_pDX11Device->Initialize(pDeviceContext, hwnd, width, height)) return false;

		m_pDX11DeviceContext = std::make_unique<CDX11DeviceContext>(pDeviceContext);

		m_pRendering = std::make_unique<CDX11ForwardRendering>(m_pDX11Device.get(), m_pDX11DeviceContext.get());
		if (!m_pRendering->Initialize(width, height)) return false;

		{
			ID3D11Texture2D* pTex2D;
			if (!m_pDX11Device->GetBackBuffer(pTex2D)) return false;

			D3D11_TEXTURE2D_DESC texDesc;
			pTex2D->GetDesc(&texDesc);

			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.ViewDimension				= D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Format						= texDesc.Format;
			rtvDesc.Texture2DArray.ArraySize	= texDesc.ArraySize;

			if (!m_pDX11Device->CreateRenderTargetView(m_pRtv, pTex2D, rtvDesc)) return false;
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
			if (!m_pDX11Device->CreateTexture2D(pTex, texDesc)) return false;

			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
			dsvDesc.ViewDimension				= D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Format						= DXGI_FORMAT_D24_UNORM_S8_UINT;
			dsvDesc.Texture2D.MipSlice			= 0;
			dsvDesc.Texture2DArray.ArraySize	= texDesc.ArraySize;

			if (!m_pDX11Device->CreateDepthStencilView(m_pDsv, pTex.Get(), dsvDesc)) return false;
		}

		{
			this->CreateConstantBuffer(m_pCameraCB);
			m_pCameraCB->CreateConstantBuffer(sizeof(CameraCB));

			this->CreateConstantBuffer(m_pDirectionalLightCB);
			m_pDirectionalLightCB->CreateConstantBuffer(sizeof(DirectionalLightCB));

			this->CreateConstantBuffer(m_pPointLightCB);
			m_pPointLightCB->CreateConstantBuffer(sizeof(PointLightCB));

			this->CreateConstantBuffer(m_pSpotLightCB);
			m_pSpotLightCB->CreateConstantBuffer(sizeof(SpotLightCB));
		}

		m_windowRect = RECT(width, height);

		return true;
	}

	// 終了
	void CDX11Renderer::Finalize() {

	}

	// 描画準備開始
	void CDX11Renderer::Begine(CSceneBase* pScene) {
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

				m_pCameraCB->Set((int)CB_TYPE::CAMERA);
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
				m_pDirectionalLightCB->Set((int)CB_TYPE::DIRECTIONAL_LIGHT);
				
				if (pointLightCb.numPointLight > 0) {
					m_pPointLightCB->Update(&pointLightCb);
					m_pPointLightCB->Set((int)CB_TYPE::POINT_LIGHT);
				}
				if (spotLightCb.numSpotLight > 0) {
					m_pSpotLightCB->Update(&spotLightCb);
					m_pSpotLightCB->Set((int)CB_TYPE::SPOT_LIGHT);
				}
			}
		}
	}

	// 描画
	bool CDX11Renderer::Render(CSceneBase* pScene) {

		m_pDX11DeviceContext->SetInputLayout(m_pInputLayout.Get());

		m_pRendering->Begine();
		
		pScene->Draw();

		m_pDX11DeviceContext->ClearRenderTargetView(m_pRtv.Get());
		m_pDX11DeviceContext->ClearDespthStencilView(m_pDsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL);
		m_pDX11DeviceContext->SetRenderTargetView(m_pRtv.Get(), m_pDsv.Get());

		CSprite sprite("RenderSprite");
		sprite.SetMesh("RenderSpriteMesh");
		sprite.SetMaterial("RenderSpriteMaterial");
		sprite.SetTexture(m_pRendering->GetRenderTexture());
		sprite.SetShaderPackage("Sprite.hlsl");
		sprite.InitializeResource(this);
		sprite.Draw();

		m_pRendering->End();

		return m_pDX11Device->Present();
	}

	// コンスタントバッファ作成
	void CDX11Renderer::CreateConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer) {
		pConstantBuffer = std::make_unique<CDX11ConstantBuffer>(m_pDX11Device.get(), m_pDX11DeviceContext.get());
	}

	// メッシュ作成
	void CDX11Renderer::CreateMesh(std::unique_ptr<CMeshBase>& pMesh, const char* meshName) {
		pMesh = std::make_unique<CDX11Mesh>(m_pDX11Device.get(), m_pDX11DeviceContext.get(), meshName);
	}

	// テクスチャ作成
	void CDX11Renderer::CreateTexture(std::unique_ptr<CTextureBase>& pTexture, const char* textureName) {
		FileInfo* fileInfo = CFileSystem::GetFileSystem().GetFileInfoFromMap(textureName);
		
		if (fileInfo) {
			pTexture = std::make_unique<CDX11Texture>(m_pDX11Device.get(), m_pDX11DeviceContext.get(), textureName);

			if (pTexture->LoadTexture(fileInfo->filePath.c_str())) {
				pTexture->Create();
			}
		}
	}

	// シェーダー作成
	void CDX11Renderer::CreateShader(std::unique_ptr<CShaderPackage>& pShaderPackage, const char* shaderName) {
		FileInfo* fileInfo = CFileSystem::GetFileSystem().GetFileInfoFromMap(shaderName);
		
		if (fileInfo) {
			for (const auto& info : vecShaderInfo) {
				ComPtr<ID3DBlob> pBlob;

				if (pShaderPackage->CompileShader(pBlob, fileInfo->filePath.c_str(), info.entryPoint, info.shaderModel)) {
					std::unique_ptr<CShaderBase> shader = std::make_unique<CDX11Shader>(m_pDX11Device.get(), m_pDX11DeviceContext.get(), shaderName, pBlob, info.shadeType);
					shader->Create();

					pShaderPackage->AddShaderToVec(shader);

					if (info.shadeType == SHADER_TYPE::TYPE_VERTEX) {
						if (m_layoutBufSize < pBlob->GetBufferSize()) {
							m_layoutBufSize = pBlob->GetBufferSize();

							m_pDX11Device->CreateInputLayout(m_pInputLayout, pBlob->GetBufferPointer(), pBlob->GetBufferSize());
						}
					}
				}
			}
		}
	}

} // namespace