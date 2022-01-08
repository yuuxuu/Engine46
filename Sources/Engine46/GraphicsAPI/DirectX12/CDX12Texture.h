﻿/**
 * @file CDX12Texture.h
 * @brief
 * @author 木村優
 * @date 2021/08/27
 */

#pragma once

#ifndef _CDX12_TEXTURE_H_
#define _CDX12_TEXTURE_H_

#include "../CTexture.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

    // 前方宣言
    class CDX12Device;
    class CDX12Command;

    class CDX12Texture : public CTextureBase {
    private:
        CDX12Device*                                pDX12Device;
        CDX12Command*                               pDX12Command;

        ComPtr<ID3D12Resource>                      m_pTextureResource;
        ComPtr<ID3D12DescriptorHeap>                m_SrvDescriptorHeap;
        
        ComPtr<ID3D12Resource>                      m_pCubeTextureResource;

        D3D12_CPU_DESCRIPTOR_HANDLE                 m_cubeSrvCpuHandle;
        D3D12_GPU_DESCRIPTOR_HANDLE                 m_cubeSrvGpuHandle;

        D3D12_CPU_DESCRIPTOR_HANDLE                 m_srvCpuHandle;
        D3D12_GPU_DESCRIPTOR_HANDLE                 m_srvGpuHandle;

        D3D12_CPU_DESCRIPTOR_HANDLE                 m_uavCpuHandle;
        D3D12_GPU_DESCRIPTOR_HANDLE                 m_uavGpuHandle;
            
    public:
        CDX12Texture(CDX12Device* pDevice, CDX12Command* pCommand, const char* textureName = "");
        ~CDX12Texture();

        void CreateTexture() override;
        void CreateCubeTexture() override;
        void CreateShaderResourceView() override;
        void Set(UINT slot) override;
        void SetCubeTexture(UINT slot) override;
        void SetCompute(UINT slot) override;

        void CreateTexture(D3D12_RESOURCE_DESC& rDesc, D3D12_HEAP_PROPERTIES& prop, D3D12_CLEAR_VALUE clearValue, D3D12_RESOURCE_STATES states = D3D12_RESOURCE_STATE_GENERIC_READ);
        void CreateDepthTexture(D3D12_RESOURCE_DESC& rDesc, D3D12_HEAP_PROPERTIES& prop, D3D12_CLEAR_VALUE clearValue);
        void CreateStencilTexture(D3D12_RESOURCE_DESC& rDesc, D3D12_HEAP_PROPERTIES& prop, D3D12_CLEAR_VALUE clearValue);
        void CreateShaderResourceView(ID3D12DescriptorHeap* pDescriptorHeap, UINT heapIndex);
        void CreateCubeTextureShaderResourceView(ID3D12DescriptorHeap* pDescriptorHeap, UINT heapIndex);
        void CreateUnorderedAccessBufferView(ID3D12DescriptorHeap* pDescriptorHeap, UINT heapIndex);

        ID3D12Resource* GetResource() const { return m_pTextureResource.Get(); }
    };

} // namespace

#endif