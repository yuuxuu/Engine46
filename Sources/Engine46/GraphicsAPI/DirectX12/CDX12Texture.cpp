/**
 * @file CDX12Texture.cpp
 * @brief
 * @author 木村優
 * @date 2021/08/27
 */

#include "CDX12Texture.h"
#include "CDX12Device.h"
#include "CDX12Command.h"

#include "CDX12ShaderPackage.h"

namespace Engine46 {

    // コンストラクタ
    CDX12Texture::CDX12Texture(CDX12Device* pDevice, CDX12Command* pCommand, const std::string& textureName) :
        CTextureBase(textureName),
        pDX12Device(pDevice),
        pDX12Command(pCommand)
    {}

    // デストラクタ
    CDX12Texture::~CDX12Texture()
    {}

    // テクスチャ作成
    void CDX12Texture::CreateTexture() {

        if (!m_textureData.pData) return;

        pDX12Command->ResetCommandList();

        D3D12_RESOURCE_DESC rDesc = {};

        rDesc.Width = m_textureData.width;
        rDesc.Height = m_textureData.height;
        rDesc.MipLevels = 1;
        rDesc.Format = m_textureData.format;
        rDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        rDesc.DepthOrArraySize = 1;
        rDesc.SampleDesc = { 1 , 0 };
        rDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        rDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

        CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

        pDX12Device->CreateResource(m_pTextureResource, prop, rDesc, nullptr, D3D12_RESOURCE_STATE_COPY_DEST);

        UINT64 bufSize = GetRequiredIntermediateSize(m_pTextureResource.Get(), 0, 1);
        CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufSize);
        prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

        ComPtr<ID3D12Resource> resource;
        pDX12Device->CreateResource(resource, prop, desc);

        D3D12_SUBRESOURCE_DATA subData = {};

        subData.pData = m_textureData.pData.get();
        subData.RowPitch = m_textureData.rowPitch;
        subData.SlicePitch = m_textureData.slicePitch;

        UpdateSubresources(
            pDX12Command->GetCommandList(),
            m_pTextureResource.Get(),
            resource.Get(),
            0,
            0,
            1,
            &subData);

        CD3DX12_RESOURCE_BARRIER rBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
            m_pTextureResource.Get(),
            D3D12_RESOURCE_STATE_COPY_DEST,
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

        pDX12Command->GetCommandList()->ResourceBarrier(1, &rBarrier);

        pDX12Command->ExecuteComandLists();
    }

    // キューブテクスチャ作成
    void CDX12Texture::CreateCubeTexture() {
        
        if (!m_cubeTextureData.pDatas[0]) {
            TextureConvertToCubeMapTexture();
        }

        pDX12Command->ResetCommandList();

        D3D12_RESOURCE_DESC rDesc = {};

        rDesc.Width = m_cubeTextureData.width;
        rDesc.Height = m_cubeTextureData.height;
        rDesc.MipLevels = 1;
        rDesc.Format = m_cubeTextureData.format;
        rDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        rDesc.DepthOrArraySize = CUBE_FACE_MAX;
        rDesc.SampleDesc = { 1 , 0 };
        rDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        rDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

        CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

        pDX12Device->CreateResource(m_pCubeTextureResource, prop, rDesc, nullptr, D3D12_RESOURCE_STATE_COPY_DEST);

        UINT64 bufSize = GetRequiredIntermediateSize(m_pCubeTextureResource.Get(), 0, CUBE_FACE_MAX);
        CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufSize);
        prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

        ComPtr<ID3D12Resource> resource;
        pDX12Device->CreateResource(resource, prop, desc);

        D3D12_SUBRESOURCE_DATA subData[CUBE_FACE_MAX] = {};

        for (int i = 0; i < CUBE_FACE_MAX; ++i) {
            subData[i].pData = m_cubeTextureData.pDatas[i].get();
            subData[i].RowPitch = m_cubeTextureData.rowPitch;
            subData[i].SlicePitch = m_cubeTextureData.slicePitch;
        }

        UpdateSubresources(
            pDX12Command->GetCommandList(),
            m_pCubeTextureResource.Get(),
            resource.Get(),
            0,
            0,
            CUBE_FACE_MAX,
            &subData[0]);

        CD3DX12_RESOURCE_BARRIER rBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
            m_pCubeTextureResource.Get(),
            D3D12_RESOURCE_STATE_COPY_DEST,
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

        pDX12Command->GetCommandList()->ResourceBarrier(1, &rBarrier);

        pDX12Command->ExecuteComandLists();
    }

    void CDX12Texture::CreateShaderResourceView() {

        D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};

        dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        dhDesc.NumDescriptors = 1;
        dhDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        pDX12Device->CreateDescriptorHeap(m_SrvDescriptorHeap, dhDesc);

        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = m_textureData.format;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;

        m_srvCpuHandle = m_SrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

        pDX12Device->CreateShaderResourceView(m_pTextureResource.Get(), srvDesc, m_srvCpuHandle);
    }

    // テクスチャをシェーダーへ設定
    void CDX12Texture::Set(UINT slot) {
        if (m_srvGpuHandle.ptr != 0) {
            pDX12Command->SetGraphicsRootDescriptorTable(slot, m_srvGpuHandle);
        }
    }
    // キューブテクスチャをシェーダーへ設定
    void CDX12Texture::SetCubeTexture(UINT slot) {
        if (m_cubeSrvGpuHandle.ptr != 0) {
            pDX12Command->SetGraphicsRootDescriptorTable(slot, m_cubeSrvGpuHandle);
        }
    }

    // テクスチャをシェーダーへ設定
    void CDX12Texture::SetCompute(UINT slot) {
        if (m_uavGpuHandle.ptr != 0) {
            pDX12Command->SetComputeRootDescriptorTable(slot, m_uavGpuHandle);
        }
        else if (m_srvGpuHandle.ptr != 0) {
            pDX12Command->SetComputeRootDescriptorTable(slot, m_srvGpuHandle);
        }
    }

    // テクスチャ作成
    void CDX12Texture::CreateTexture(D3D12_RESOURCE_DESC& rDesc, D3D12_HEAP_PROPERTIES& prop, D3D12_CLEAR_VALUE clearValue, D3D12_RESOURCE_STATES states) {

        pDX12Device->CreateResource(m_pTextureResource, prop, rDesc, &clearValue, states);

        m_textureData.width = (UINT)rDesc.Width;
        m_textureData.height = (UINT)rDesc.Height;
        m_textureData.format = rDesc.Format;
    }

    // デプステクスチャ作成
    void CDX12Texture::CreateDepthTexture(D3D12_RESOURCE_DESC& rDesc, D3D12_HEAP_PROPERTIES& prop, D3D12_CLEAR_VALUE clearValue) {

        CreateTexture(rDesc, prop, clearValue);

        m_textureData.width = (UINT)rDesc.Width;
        m_textureData.height = (UINT)rDesc.Height;
        m_textureData.format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    }

    // ステンシルテクスチャ作成
    void CDX12Texture::CreateStencilTexture(D3D12_RESOURCE_DESC& rDesc, D3D12_HEAP_PROPERTIES& prop, D3D12_CLEAR_VALUE clearValue) {

        CreateTexture(rDesc, prop, clearValue);

        m_textureData.width = (UINT)rDesc.Width;
        m_textureData.height = (UINT)rDesc.Height;
        m_textureData.format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
    }

    // シェーダーリソースビュー作成
    void CDX12Texture::CreateShaderResourceView(ID3D12DescriptorHeap* pDescriptorHeap, UINT heapIndex) {

        if (!m_pTextureResource) return;

        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = m_textureData.format;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;

        UINT64 heapSize = pDX12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        m_srvCpuHandle = pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        m_srvCpuHandle.ptr += heapSize * heapIndex;

        m_srvGpuHandle = pDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
        m_srvGpuHandle.ptr += heapSize * heapIndex;

        pDX12Device->CreateShaderResourceView(m_pTextureResource.Get(), srvDesc, m_srvCpuHandle);
    }

    // キューブマップ用シェーダーリソースビュー作成
    void CDX12Texture::CreateCubeTextureShaderResourceView(ID3D12DescriptorHeap* pDescriptorHeap, UINT heapIndex) {
        
        if (!m_pCubeTextureResource) return;

        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = m_textureData.format;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
        srvDesc.Texture2D.MipLevels = 1;

        UINT64 heapSize = pDX12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        m_cubeSrvCpuHandle = pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        m_cubeSrvCpuHandle.ptr += heapSize * heapIndex;

        m_cubeSrvGpuHandle = pDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
        m_cubeSrvGpuHandle.ptr += heapSize * heapIndex;

        pDX12Device->CreateShaderResourceView(m_pCubeTextureResource.Get(), srvDesc, m_cubeSrvCpuHandle);
    }

    // アンオーダードアクセスバッファ作成
    void CDX12Texture::CreateUnorderedAccessBufferView(ID3D12DescriptorHeap* pDescriptorHeap, UINT heapIndex) {
        
        if (!m_pTextureResource) return;
        
        D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

        uavDesc.Format = DXGI_FORMAT_UNKNOWN;
        uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
        uavDesc.Buffer.NumElements = 0;
        uavDesc.Buffer.StructureByteStride = 0;

        UINT64 heapSize = pDX12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        m_uavCpuHandle = pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        m_uavCpuHandle.ptr += heapSize * heapIndex;

        m_uavGpuHandle = pDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
        m_uavGpuHandle.ptr += heapSize * heapIndex;

        pDX12Device->CreateUnorderedAccessView(m_pTextureResource.Get(), uavDesc, m_uavCpuHandle);
    }

} // namespace
