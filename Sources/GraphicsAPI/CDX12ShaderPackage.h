/**
 * @file CDX12ShaderPackage.h
 * @brief
 * @author 木村優
 * @date 2021/08/28
 */

#pragma once

#ifndef _CDX12_SHADER_PCKAGE_H_
#define _CDX12_SHADER_PCKAGE_H_

#include "../Engine46/CShaderPackage.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	// 前方宣言
	class CDX12Device;
	class CDX12Command;

	enum class MyRootSignature_01 {
		CBV_WORLD,
		CBV_MATERIAL,
		CBV_CAMERA,
		CBV_DirectionalLight,
		CBV_PointLight,
		CBV_SpotLight,
		SRV_diffuse,
	};

	const UINT STATIC_MAX = 4;

	class CDX12ShaderPackage : public CShaderPackage {
	private:
		CDX12Device*					pDX12Device;
		CDX12Command*					pDX12Command;
		ID3D12DescriptorHeap*			pDescriptorHeap;

		ComPtr<ID3D12RootSignature>		m_pGraphicsRootSignature;
		ComPtr<ID3D12PipelineState>		m_pGraphicsPiplineState;

		ComPtr<ID3D12RootSignature>		m_pComputeRootSignature;
		ComPtr<ID3D12PipelineState>		m_pComputePiplineState;


	public:
		CDX12ShaderPackage(CDX12Device* pDevice, CDX12Command* pCommand, const char* shaderName);
		~CDX12ShaderPackage();

		bool InitializeGraphics(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc);
		bool InitializeCompute(D3D12_COMPUTE_PIPELINE_STATE_DESC& cpuDesc);

		bool Initialize() override;

		void SetShader() override;

		void CopyDescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, UINT destIndex);

		void SetShaderByteCode(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc);
		void SetRTVFormats(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc);
		void SetDepthStencilState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc);
		void SetRasterizerState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc);
		void SetBlendState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc);

		void SetDescriptorHeap(ID3D12DescriptorHeap* pDescriptorHeap) { this->pDescriptorHeap = pDescriptorHeap; }
	};

} // namespace

#endif