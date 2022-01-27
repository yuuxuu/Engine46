/**
 * @file CDX12TiledForwardRendering.h
 * @brief
 * @author 木村優
 * @date 2022/01/08
 */

#pragma once

#ifndef _CDX12_TILED_FORWARD_RENDERING_H_
#define _CDX12_TILED_FORWARD_RENDERING_H_

#include "../CRendering.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

    // 前方宣言
    class CDX12Device;
    class CDX12Command;
    class CDX12Texture;
    class CUnorderedAccessBufferBase;

    class CDX12TiledForwardRendering : public CRenderingBase {
    private:
        CDX12Device*                                pDX12Device;
        CDX12Command*                               pDX12Command;

        CDX12Texture*                               pDX12RenderTexture;

        ComPtr<ID3D12DescriptorHeap>                m_pRtvDescriptorHeap;
        D3D12_CPU_DESCRIPTOR_HANDLE                 m_rtvHandle;

        ComPtr<ID3D12Resource>                      m_pDsvResource;
        ComPtr<ID3D12DescriptorHeap>                m_pDsvDescriptorHeap;
        D3D12_CPU_DESCRIPTOR_HANDLE                 m_dsvHandle;

        std::unique_ptr<CUnorderedAccessBufferBase> m_pLightIndexUab;

    public:
        CDX12TiledForwardRendering(CDX12Device* pDevice, CDX12Command* pCommand);
        ~CDX12TiledForwardRendering();

        bool Initialize(UINT width, UINT height) override;
        void Begine() override;
        void End() override;

        void Rendering(CSceneBase* pScene) override;
        void RenderingForRenderScene(CSprite* pSprite, UINT x, UINT y, UINT width, UINT height) override;

        void LightCulling_CS(CDX12Texture* pDX12DepthTexture);

        CDX12Texture* GetRenderTexture() const { return pDX12RenderTexture; }
    };

} // namespace

#endif