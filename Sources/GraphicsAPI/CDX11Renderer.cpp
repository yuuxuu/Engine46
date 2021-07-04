/**
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
#include "CDX11Material.h"
#include "CDX11Shader.h"
#include "CDX11Texture.h"

#include "../Engine46/CGameSystem.h"
#include "../Engine46/CFileSystem.h"
#include "../Engine46/CShaderManager.h"
#include "../Engine46/CSprite.h"

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

		m_windowRect = RECT(width, height);

		return true;
	}

	// 終了
	void CDX11Renderer::Finalize() {

	}

	// 描画
	bool CDX11Renderer::Render(CSceneBase* pScene) {

		m_pDX11DeviceContext->SetInputLayout(m_pInputLayout.Get());

		m_pRendering->Begine();
		
		pScene->Draw();
		
		m_pRendering->End();

		m_pDX11DeviceContext->ClearRenderTargetView(m_pRtv.Get());
		m_pDX11DeviceContext->ClearDespthStencilView(m_pDsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL);
		m_pDX11DeviceContext->SetRenderTargetView(m_pRtv.Get(), m_pDsv.Get());

		CSprite sprite("RenderSprite");
		sprite.InitializeResource(this);
		sprite.SetTexture(m_pRendering->GetRenderTexture());
		sprite.SetShaderPackage("Sprite.hlsl");
		sprite.Draw();

		return m_pDX11Device->Present();
	}

	// コンスタントバッファ作成
	void CDX11Renderer::CreateConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer) {
		pConstantBuffer = std::make_unique<CDX11ConstantBuffer>(m_pDX11Device.get(), m_pDX11DeviceContext.get());
	}

	// メッシュ作成
	void CDX11Renderer::CreateMesh(std::unique_ptr<CMeshBase>& pMesh) {
		pMesh = std::make_unique<CDX11Mesh>(m_pDX11Device.get(), m_pDX11DeviceContext.get());
	}

	// マテリアル作成
	void CDX11Renderer::CreateMaterial(std::unique_ptr<CMaterialBase>& pMaterial) {
		pMaterial = std::make_unique<CDX11Material>(m_pDX11Device.get(), m_pDX11DeviceContext.get());
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

					if (m_layoutBufSize < pBlob->GetBufferSize()) {
						m_layoutBufSize = pBlob->GetBufferSize();

						m_pDX11Device->CreateInputLayout(m_pInputLayout, pBlob->GetBufferPointer(), pBlob->GetBufferSize());
					}
				}
			}
		}
	}

} // namespace