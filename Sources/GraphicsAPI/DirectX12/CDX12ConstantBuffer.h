/**
 * @file CDX12ConstantBuffer.h
 * @brief
 * @author 木村優
 * @date 2021/08/25
 */

#pragma once

#ifndef _CCDX12_CONSTANTBUFFER_H_
#define _CCDX12_CONSTANTBUFFER_H_

#include "Engine46/CConstantBuffer.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

    // 前方宣言
    class CDX12Device;
    class CDX12Command;

    class CDX12ConstantBuffer : public CConstantBufferBase {
    private:
        CDX12Device* pDX12Device;
        CDX12Command* pDX12Command;

        ComPtr<ID3D12Resource>          m_pCbResource;
        ComPtr<ID3D12DescriptorHeap>    m_pCbDescriptorHeap;

        void* m_mappedBuffer;

        D3D12_CPU_DESCRIPTOR_HANDLE     m_cpuHandle;
        D3D12_GPU_DESCRIPTOR_HANDLE	    m_gpuHandle;

        UINT                            m_byteSize;

    public:
        CDX12ConstantBuffer(CDX12Device* pDevice, CDX12Command* pCommand);
        ~CDX12ConstantBuffer();

        void CreateConstantBuffer(UINT byteWidth) override;
        void CreateConstantBufferView() override;
        void Update(void* srcData) override;
        void Set(UINT slot) override;
        void SetCompute(UINT slot) override;

        void CreateConstantBufferView(ID3D12DescriptorHeap* pDescriptorHeap, UINT heapIndex);

        ID3D12DescriptorHeap* GetDescriptorHeap() const { return m_pCbDescriptorHeap.Get(); }
    };

} // namespace

#endif

