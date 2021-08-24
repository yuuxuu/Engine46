/**
 * @file CDX12Command.h
 * @brief
 * @author 木村優
 * @date 2021/08/18
 */

#pragma once

#ifndef _CDX12COMMAND_H_
#define _CDX12COMMAND_H_

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	// 前方宣言
	class CDX12Device;

	class CDX12Command {
	private:
		ComPtr<ID3D12CommandQueue>			m_pCommandQueue;

		ComPtr<ID3D12GraphicsCommandList>	m_pCommandList;

		ComPtr<ID3D12CommandAllocator>		m_pCommandAllocator;

		ComPtr<ID3D12Fence>					m_pFence;

		HANDLE								m_fence_event;

		UINT								m_frames;

	public:
		explicit CDX12Command(ComPtr<ID3D12CommandQueue>& pCommandQueue);
		~CDX12Command();

		bool Initialize(CDX12Device* pDevice);

		bool CloseCommandList();

		bool ResetCommandList();

		void ExecuteComandLists();

		void WaitForPreviousFrame(ID3D12CommandQueue* pCommandQueue);
		
		void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE handle);

		void ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE handle, D3D12_CLEAR_FLAGS clearFlags);

		void SetRect(UINT width, UINT height);

		void SetViewPort(UINT width, UINT height);

		void SetRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsHandle);

		void SetResourceBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

		void SetDescriptorHeaps(ID3D12DescriptorHeap* pDescriptorHeap);

		void SetRootSignature(ID3D12RootSignature* pRootSignature);

		void SetPipelineState(ID3D12PipelineState* pPipelineState);

		void SetRootDescriptorTable(UINT index, D3D12_GPU_DESCRIPTOR_HANDLE handle);
		
		void SetRootConstantBufferView(UINT index, D3D12_GPU_VIRTUAL_ADDRESS address);
		
		void SetRootShaderResourceView(UINT index, D3D12_GPU_VIRTUAL_ADDRESS address);
		
		void SetRootUnorderdAccessView(UINT index, D3D12_GPU_VIRTUAL_ADDRESS address);
	};

} // namespace

#endif



