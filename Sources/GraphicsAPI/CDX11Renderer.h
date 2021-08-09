/**
 * @file CDX11Renderer.h
 * @brief
 * @author 木村優
 * @date 2021/05/02
 */

#pragma once

#ifndef _CDX11RENDERER_H_
#define _CDX11RENDERER_H_

#include "../Engine46/CRenderer.h"

namespace Engine46 {

	// 前方宣言
	class CDX11Device;
	class CDX11DeviceContext;

	class CDX11Renderer : public CRendererBase {
	private:
		std::unique_ptr<CDX11Device>			m_pDX11Device;
		std::unique_ptr<CDX11DeviceContext>		m_pDX11DeviceContext;

		ComPtr<ID3D11RenderTargetView>			m_pRtv;
		
		ComPtr<ID3D11DepthStencilView>			m_pDsv;

		ComPtr<ID3D11InputLayout>				m_pInputLayout;

		size_t									m_layoutBufSize;
	public:
		CDX11Renderer();
		~CDX11Renderer();

		bool Initialize(HWND hwnd, UINT width, UINT height) override;
		
		void Finalize() override;

		void Begine(CSceneBase* pScene) override;

		bool Render(CSceneBase* pScene) override;

		void CreateConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer) override;
		void CreateMesh(std::unique_ptr<CMeshBase>& pMesh, const char* meshName) override;
		void CreateTexture(std::unique_ptr<CTextureBase>& pTexture, const char* textureName) override;
		void CreateShader(std::unique_ptr<CShaderPackage>& pShaderPackage, const char* shaderName) override;

		CDX11Device* GetDX11Device() const { return m_pDX11Device.get(); }

		CDX11DeviceContext* GetDX11DeviceContext() const { return m_pDX11DeviceContext.get(); }
	};

} // namespace

#endif