/**
 * @file CDX12UnorderedAccessBuffer.cpp
 * @brief
 * @author 木村優
 * @date 2021/09/29
 */

#include "CDX12UnorderedAccessBuffer.h"
#include "CDX12Device.h"
#include "CDX12Command.h"
#include "CDX12ShaderPackage.h"

namespace Engine46 {

    // コンストラクタ
    CDX12UnorderedAccessBuffer::CDX12UnorderedAccessBuffer(CDX12Device* pDevice, CDX12Command* pCommand) :
        pDX12Device(pDevice),
        pDX12Command(pCommand)
    {}

    // デストラクタ
    CDX12UnorderedAccessBuffer::~CDX12UnorderedAccessBuffer(){
        if(m_pUabResource) m_pUabResource->Unmap(0, nullptr);
    }

    // アンオーダードアクセスバッファ作成
    void CDX12UnorderedAccessBuffer::CreateUnorderedAccessBuffer(UINT byteWidth, UINT byteSize) {

        CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_CUSTOM);

        D3D12_RESOURCE_DESC rDesc = {};

        rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        rDesc.Width = ((UINT64)byteWidth * (UINT64)byteSize + 0xff) & ~0xff;
        rDesc.Height = 1;
        rDesc.DepthOrArraySize = 1;
        rDesc.MipLevels = 1;
        rDesc.Format = DXGI_FORMAT_UNKNOWN;
        rDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
        rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        rDesc.SampleDesc.Count = 1;

        pDX12Device->CreateResource(m_pUabResource, prop, rDesc, nullptr);

        D3D12_RANGE range = { 0, 0 };

        HRESULT hr = m_pUabResource->Map(0, &range, &m_mappedBuffer);
        if (FAILED(hr)) {
            m_pUabResource->Unmap(0, nullptr);

            return;
        }

        m_byteWidth = byteWidth;
        m_byteSize = byteSize;
    }

    // アンオーダードアクセスバッファビュー作成
    void CDX12UnorderedAccessBuffer::CreateUnorderedAccessBufferView() {
        D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};

        dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        dhDesc.NumDescriptors = 1;
        dhDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        pDX12Device->CreateDescriptorHeap(m_pUabDescriptorHeap, dhDesc);

        D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

        uavDesc.Format = DXGI_FORMAT_UNKNOWN;
        uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
        uavDesc.Buffer.NumElements = m_byteWidth;
        uavDesc.Buffer.StructureByteStride = m_byteSize;

        m_uavCpuHandle = m_pUabDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

        pDX12Device->CreateUnorderedAccessView(m_pUabResource.Get(), uavDesc, m_uavCpuHandle);
    }

    // アンオーダードアクセスバッファをシェーダーへ設定
    void CDX12UnorderedAccessBuffer::Set(UINT slot) {
        if (m_srvGpuHandle.ptr != 0) {
            pDX12Command->SetGraphicsRootDescriptorTable(slot, m_srvGpuHandle);
        }
    }

    // アンオーダードアクセスバッファをシェーダーへ設定
    void CDX12UnorderedAccessBuffer::SetCompute(UINT slot) {
        if (m_uavGpuHandle.ptr != 0) {
            pDX12Command->SetComputeRootDescriptorTable(slot, m_uavGpuHandle);
        }
    }

    // ソースバッファをマッピングバッファへ書き込み
    void CDX12UnorderedAccessBuffer::WirteBufferData(void* srcData) {
        memcpy(m_mappedBuffer, srcData, m_byteWidth * m_byteSize);
    }

    // リソースを設定
    void CDX12UnorderedAccessBuffer::SetResource(ComPtr<ID3D12Resource>& pResource) {
        if (pResource) {
            m_pUabResource.Swap(pResource);
        }
    }

    // アンオーダードアクセスバッファビュー作成
    void CDX12UnorderedAccessBuffer::CreateUnorderedAccessBufferView(ID3D12DescriptorHeap* pDescriptorHeap, UINT heapIndex) {
        D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

        uavDesc.Format = DXGI_FORMAT_UNKNOWN;
        uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
        uavDesc.Buffer.NumElements = m_byteSize;
        uavDesc.Buffer.StructureByteStride = m_byteWidth;

        UINT heapSize = pDX12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        m_uavCpuHandle = pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        m_uavCpuHandle.ptr += heapSize * heapIndex;

        m_uavGpuHandle = pDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
        m_uavGpuHandle.ptr += (UINT64)heapSize * heapIndex;

        pDX12Device->CreateUnorderedAccessView(m_pUabResource.Get(), uavDesc, m_uavCpuHandle);
    }

    // アンオーダードアクセスバッファビュー作成
    void CDX12UnorderedAccessBuffer::CreateUnorderedAccessBufferView(ID3D12Resource* pResource, D3D12_UNORDERED_ACCESS_VIEW_DESC& uavDesc, ID3D12DescriptorHeap* pDescriptorHeap, UINT heapIndex) {

        if (pDescriptorHeap) {
            UINT heapSize = pDX12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

            m_uavCpuHandle = pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
            m_uavCpuHandle.ptr += heapSize * heapIndex;

            m_uavGpuHandle = pDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
            m_uavGpuHandle.ptr += (UINT64)heapSize * heapIndex;
        }

        if (pResource) {
            D3D12_RESOURCE_DESC rDesc = pResource->GetDesc();

            m_texWidth = (UINT)rDesc.Width;
            m_texHeight = (UINT)rDesc.Height;
            m_texFormat = rDesc.Format;

            pDX12Device->CreateUnorderedAccessView(pResource, uavDesc, m_uavCpuHandle);
        }
        else if(m_pUabResource){
            D3D12_RESOURCE_DESC rDesc = m_pUabResource->GetDesc();

            m_texWidth = (UINT)rDesc.Width;
            m_texHeight = (UINT)rDesc.Height;
            m_texFormat = rDesc.Format;

            pDX12Device->CreateUnorderedAccessView(m_pUabResource.Get(), uavDesc, m_uavCpuHandle);
        }
    }

    // シェーダーリソースビュー作成
    void CDX12UnorderedAccessBuffer::CreateShaderResourceView(ID3D12DescriptorHeap* pDescriptorHeap, UINT heapIndex) {
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = m_texFormat;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;

        UINT heapSize = pDX12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        m_srvCpuHandle = pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        m_srvCpuHandle.ptr += heapSize * heapIndex;

        m_srvGpuHandle = pDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
        m_srvGpuHandle.ptr += (UINT64)heapSize * heapIndex;

        pDX12Device->CreateShaderResourceView(m_pUabResource.Get(), srvDesc, m_srvCpuHandle);
    }

} // namespace