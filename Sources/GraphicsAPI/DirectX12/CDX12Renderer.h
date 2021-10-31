/**
 * @file CDX12Renderer.h
 * @brief
 * @author 木村優
 * @date 2021/08/19
 */

#pragma once

#ifndef _CDX12RENDERER_H_
#define _CDX12RENDERER_H_

#include "../CRenderer.h"

#include "PostEffect/CDX12PostEffect.h"

#pragma comment(lib, "PostEffect.lib")

using Microsoft::WRL::ComPtr;

namespace Engine46 {

    // 前方宣言
    class CDX12Device;
    class CDX12Command;
    class CDX12Texture;
    class CDX12UnorderedAccessBuffer;

    class CDX12Renderer : public CRendererBase {
    private:
        std::unique_ptr<CDX12Device>        m_pDX12Device;
        std::unique_ptr<CDX12Command>       m_pDX12Command;

        std::unique_ptr<CDX12PostEffect>    m_pDX12PostEffect;

        ComPtr<ID3D12Resource>              m_pRtvResource[2];
        ComPtr<ID3D12DescriptorHeap>        m_pRtvDescriptorHeap;
        D3D12_CPU_DESCRIPTOR_HANDLE         m_rtvHandle[2];

        ComPtr<ID3D12Resource>              m_pDsvResource;
        ComPtr<ID3D12DescriptorHeap>        m_pDsvDescriptorHeap;
        D3D12_CPU_DESCRIPTOR_HANDLE         m_dsvHandle;

        ComPtr<ID3D12DescriptorHeap>        m_pCbDescriptorHeap;

        UINT                                m_descriptorHeapOffsetIndex;

    public:
        CDX12Renderer();
        ~CDX12Renderer();

        bool Initialize(HWND hwnd, UINT width, UINT height) override;
        void Finalize() override;
        void Begine(CSceneBase* pScene) override;
        bool Render(CSceneBase* pScene) override;

        void Reset();
        void SetSceneConstantBuffers(UINT startSlot);

        void CreateConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer, UINT byteWidth) override;
        void CreateUnorderedAccessBuffer(std::unique_ptr<CUnorderedAccessBufferBase>& pUnorderedAccessBuffer, UINT byteWidth, UINT byteSize) override;
        void CreateMesh(std::unique_ptr<CMeshBase>& pMesh, const char* meshName) override;
        void CreateTexture(std::unique_ptr<CTextureBase>& pTexture, const char* textureName) override;
        void CreateShader(std::unique_ptr<CShaderPackage>& pShaderPackage, const char* shaderName) override;

        void CreateResource(ComPtr<ID3D12Resource>& pResource, D3D12_RESOURCE_DESC& rDesc);

        void CreateRenderTexture(std::unique_ptr<CDX12Texture>& pDX12RenderTexture, D3D12_RESOURCE_DESC& rDesc, D3D12_CLEAR_VALUE& clearValue, TextureType type = TextureType::Render);

        void CreateShaderResourceView(CDX12Texture* pDX12Texture);
        void CreateUnorderedAccessBufferView(CDX12Texture* pDX12Texture);

        void CreateUnorderedAccessBuffer(std::unique_ptr<CDX12UnorderedAccessBuffer>& pUnorderedAccessBuffer);
        void CreateUnorderedAccessBufferView(CDX12UnorderedAccessBuffer* pUnorderedAccessBuffer, ComPtr<ID3D12Resource>& pResource, D3D12_UNORDERED_ACCESS_VIEW_DESC& uavDesc);
        void CreateUnorderedAccessBufferView(CDX12UnorderedAccessBuffer* pUnorderedAccessBuffer, ID3D12Resource* pResource, D3D12_UNORDERED_ACCESS_VIEW_DESC& uavDesc);
    };
}

#endif

