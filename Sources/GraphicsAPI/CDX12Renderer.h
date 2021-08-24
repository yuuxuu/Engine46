/**
 * @file CDX12Renderer.h
 * @brief
 * @author 木村優
 * @date 2021/08/19
 */

#pragma once

#ifndef _CDX12RENDERER_H_
#define _CDX12RENDERER_H_

#include "../Engine46\CRenderer.h"

namespace Engine46 {

	// 前方宣言
	class CDX12Device;
	class CDX12Command;

	class CDX12Renderer : public CRendererBase {
	private:
		std::unique_ptr<CDX12Device>	m_pDX12Device;
		std::unique_ptr<CDX12Command>	m_pDX12Command;

		ComPtr<ID3D12Resource>			m_pRtvResource[2];
		ComPtr<ID3D12DescriptorHeap>	m_pD3D12RtvDescriptorHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE		m_rtvHandle[2];

		ComPtr<ID3D12Resource>			m_pDsvResource;
		ComPtr<ID3D12DescriptorHeap>	m_pD3D12DsvDescriptorHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE		m_dsvHandle;

	public:
		CDX12Renderer();
		~CDX12Renderer();

		bool Initialize(HWND hwnd, UINT width, UINT height);
		void Finalize() override;
		void Begine(CSceneBase* pScene) override;
		bool Render(CSceneBase* pScene) override;

		void CreateConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer);
		void CreateMesh(std::unique_ptr<CMeshBase>& pMesh, const char* meshName);
		void CreateTexture(std::unique_ptr<CTextureBase>& pTexture, const char* textureName);
		void CreateShader(std::unique_ptr<CShaderPackage>& pShaderPackage, const char* shaderName);
	};
}

#endif

