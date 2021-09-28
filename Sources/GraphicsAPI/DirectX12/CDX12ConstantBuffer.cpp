/**
 * @file CDX12ConstantBuffer.cpp
 * @brief
 * @author 木村優
 * @date 2021/08/25
 */

#include "CDX12ConstantBuffer.h"

#include "CDX12Device.h"
#include "CDX12Command.h"

namespace Engine46 {

    // コンストラクタ
    CDX12ConstantBuffer::CDX12ConstantBuffer(CDX12Device* pDevice, CDX12Command* pCommand) :
        pDX12Device(pDevice),
        pDX12Command(pCommand),
        m_bufSize(0)
    {}

    // デストラクタ
    CDX12ConstantBuffer::~CDX12ConstantBuffer() {
        if (m_pCbResource) m_pCbResource->Unmap(0, nullptr);
    }

    // コンスタントバッファ作成
    void CDX12ConstantBuffer::CreateConstantBuffer(UINT byteWidth) {

        CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

        D3D12_RESOURCE_DESC rDesc = {};

        rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        rDesc.Width = ((UINT64)byteWidth + 0xff) & ~0xff;
        rDesc.Height = 1;
        rDesc.DepthOrArraySize = 1;
        rDesc.MipLevels = 1;
        rDesc.Format = DXGI_FORMAT_UNKNOWN;
        rDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        rDesc.SampleDesc.Count = 1;

        pDX12Device->CreateResource(m_pCbResource, prop, rDesc, nullptr);

        D3D12_RANGE range = { 0, 0 };

        HRESULT hr = m_pCbResource->Map(0, &range, &m_mappedBuffer);
        if (FAILED(hr)) {
            m_pCbResource->Unmap(0, nullptr);

            return;
        }

        m_bufSize = byteWidth;
    }

    // コンスタントバッファビュー作成
    void CDX12ConstantBuffer::CreateConstantBufferView() {
        D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};

        dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        dhDesc.NumDescriptors = 1;
        dhDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        pDX12Device->CreateDescriptorHeap(m_pCbDescriptorHeap, dhDesc);

        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};

        cbvDesc.SizeInBytes = (m_bufSize + 0xff) & ~0xff;
        cbvDesc.BufferLocation = m_pCbResource->GetGPUVirtualAddress();

        m_cpuHandle = m_pCbDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

        pDX12Device->CreateConstantBufferView(m_pCbResource.Get(), cbvDesc, m_cpuHandle);
    }

    // 更新
    void CDX12ConstantBuffer::Update(void* srcData) {
        memcpy(m_mappedBuffer, srcData, m_bufSize);
    }

    // コンスタントバッファをシェーダーへ設定
    void CDX12ConstantBuffer::Set(UINT slot) {

        if (m_gpuHandle.ptr != 0) {
            pDX12Command->SetRootDescriptorTable(slot, m_gpuHandle);
            return;
        }

        if (m_pCbDescriptorHeap) {
            //pDX12Command->SetDescriptorHeaps(m_pCbDescriptorHeap.Get());

            //pDX12Command->SetRootDescriptorTable(slot, m_pCbDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
        }
    }

    // コンスタントバッファビュー作成
    void CDX12ConstantBuffer::CreateConstantBufferView(ID3D12DescriptorHeap* pDescriptorHeap, UINT heapIndex) {
        D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc = {};

        cbDesc.SizeInBytes = (m_bufSize + 0xff) & ~0xff;
        cbDesc.BufferLocation = m_pCbResource->GetGPUVirtualAddress();

        UINT heapSize = pDX12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        m_cpuHandle = pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        m_cpuHandle.ptr += heapSize * heapIndex;

        m_gpuHandle = pDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
        m_gpuHandle.ptr += (UINT64)heapSize * heapIndex;

        pDX12Device->CreateConstantBufferView(m_pCbResource.Get(), cbDesc, m_cpuHandle);
    }

} // namespace