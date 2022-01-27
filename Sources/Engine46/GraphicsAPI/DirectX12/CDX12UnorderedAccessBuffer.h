/**
 * @file CDX12UnorderedAccessBuffer.h
 * @brief
 * @author 木村優
 * @date 2021/09/29
 */

#pragma once

#ifndef _CCDX12_UNORDERERD_ACCESS_BUFFER_H_
#define _CCDX12_UNORDERERD_ACCESS_BUFFER_H_

#include "../CUnorderedAccessBuffer.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

    // 前方宣言
    class CDX12Device;
    class CDX12Command;

    class CDX12UnorderedAccessBuffer : public CUnorderedAccessBufferBase {
    private:
        CDX12Device*                    pDX12Device;
        CDX12Command*                   pDX12Command;

        ComPtr<ID3D12Resource>          m_pUabResource;
        ComPtr<ID3D12DescriptorHeap>    m_pUabDescriptorHeap;

        void*                           m_mappedBuffer;

        D3D12_CPU_DESCRIPTOR_HANDLE     m_srvCpuHandle;
        D3D12_GPU_DESCRIPTOR_HANDLE	    m_srvGpuHandle;

        D3D12_CPU_DESCRIPTOR_HANDLE     m_uavCpuHandle;
        D3D12_GPU_DESCRIPTOR_HANDLE	    m_uavGpuHandle;

        D3D12_VERTEX_BUFFER_VIEW        m_vbView;

    public:
        CDX12UnorderedAccessBuffer(CDX12Device* pDevice, CDX12Command* pCommand);
        ~CDX12UnorderedAccessBuffer();

        void CreateUnorderedAccessBuffer(UINT byteWidth, UINT byteSize) override;
        void CreateUnorderedAccessBufferView() override;
        void WriteBufferData(void* srcData, UINT size) override;
        void Set(UINT slot) override;
        void SetCompute(UINT slot) override;
        void Dispatch(UINT dispatchX, UINT dispatchY, UINT dispatchZ) override;

        void Draw() override;
        void CreateVertexBufferView() override;

        void SetResource(ComPtr<ID3D12Resource>& pResource);

        void CreateUnorderedAccessBufferView(ID3D12DescriptorHeap* pDescriptorHeap, UINT heapIndex);
        void CreateUnorderedAccessBufferView(ID3D12Resource* pResource, D3D12_UNORDERED_ACCESS_VIEW_DESC& uavDesc, ID3D12DescriptorHeap* pDescriptorHeap, UINT heapIndex);
        void CreateShaderResourceView(ID3D12DescriptorHeap* pDescriptorHeap, UINT heapIndex);

        ID3D12Resource* GetResource() const { return m_pUabResource.Get(); }

        void* GetMappedBuffer() const { return m_mappedBuffer; }
    };

} // namespace

#endif
