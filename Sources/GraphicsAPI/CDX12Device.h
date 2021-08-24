/**
 * @file CDX12Device.h
 * @brief
 * @author 木村優
 * @date 2021/08/17
 */

#pragma once

#ifndef _CDX12DEVICE_H_
#define _CDX12DEVICE_H_

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	class CDX12Device {
	private:
		ComPtr<ID3D12Device>		m_pDevice;

		ComPtr<ID3D12CommandQueue>	m_pCommandQueue;

		ComPtr<IDXGISwapChain3>		m_pSwapChain;

	public:
		CDX12Device();
		~CDX12Device();

		bool Initialize(ComPtr<ID3D12CommandQueue>& pCommandQueue, HWND hwnd, UINT width, UINT height);

		bool Present();

		bool CreateFence(ComPtr<ID3D12Fence>& pFence);

		bool CreateCommandQueue(ComPtr<ID3D12CommandQueue>& pCOmmandQueue, D3D12_COMMAND_QUEUE_DESC cqDesc);

		bool CreateCommandAllcator(ComPtr<ID3D12CommandAllocator>& pCommandAllocator, D3D12_COMMAND_LIST_TYPE type);
			 
		bool CreateCommandList(ComPtr<ID3D12GraphicsCommandList>& pCommandList, ID3D12CommandAllocator* pAllocator, D3D12_COMMAND_LIST_TYPE type);

		bool CreateRootSignature(ComPtr<ID3D12RootSignature>& pRootSignature, ID3DBlob* pSignatureBlob = nullptr, D3D12_ROOT_SIGNATURE_DESC* rsDesc = nullptr);
			 
		bool CreateGraphicsPipelineState(ComPtr<ID3D12PipelineState>& pPipelineState, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc);
			 
		bool CreateComputePipelineState(ComPtr<ID3D12PipelineState>& pPipelineState, D3D12_COMPUTE_PIPELINE_STATE_DESC& cpsDesc);

		bool CreateDescriptorHeap(ComPtr<ID3D12DescriptorHeap>& pDescriptorHeap, D3D12_DESCRIPTOR_HEAP_DESC& dhDesc);

		bool CreateResource(ComPtr<ID3D12Resource>& pResource, D3D12_RESOURCE_DESC& rDesc, D3D12_CLEAR_VALUE& clearValue);

		bool CreateRenderTargetView(ComPtr<ID3D12Resource>& pResource, D3D12_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT index);

		void CreateDepthStencilView(ID3D12Resource* pResource, D3D12_DEPTH_STENCIL_VIEW_DESC* dsvDesc, D3D12_CPU_DESCRIPTOR_HANDLE handle);
			 
		void CreateConstantBufferView(ID3D12Resource* pResource, D3D12_CONSTANT_BUFFER_VIEW_DESC& cbvDesc, D3D12_CPU_DESCRIPTOR_HANDLE handle);
			 
		void CreateShaderResourceView(ID3D12Resource* pResource, D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc, D3D12_CPU_DESCRIPTOR_HANDLE handle);
			 
		void CreateUnorderedAccessView(ID3D12Resource* pResource, D3D12_UNORDERED_ACCESS_VIEW_DESC& uavDesc, D3D12_CPU_DESCRIPTOR_HANDLE handle);
		
		void CreateSampler(D3D12_CPU_DESCRIPTOR_HANDLE handle, D3D12_SAMPLER_DESC& sDesc);

		UINT GetCurrentBackBufferIndex() {
			return m_pSwapChain->GetCurrentBackBufferIndex();
		}

		UINT GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE type) {
			return m_pDevice->GetDescriptorHandleIncrementSize(type);
		}
	};

} // namespace

#endif

