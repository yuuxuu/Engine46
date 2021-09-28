/**
 * @file CDX12DeferredRendering.h
 * @brief
 * @author 木村優
 * @date 2021/09/16
 */

#pragma once

#ifndef _CDX12_DEFERRED_RENDERING_H_
#define _CDX12_DEFERRED_RENDERING_H_

#include "../CRendering.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

    // 前方宣言
    class CDX12Device;
    class CDX12Command;
    class CDX12Texture;

    class CDX12DeferredRenderig : public CRenderingBase {
    private:
        CDX12Device* pDX12Device;
        CDX12Command* pDX12Command;

        std::vector<std::unique_ptr<CDX12Texture>>  m_pVecRenderTex;

        ComPtr<ID3D12DescriptorHeap>                m_pRtvDescriptorHeap;
        std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>    m_vecRtvHandle;

        ComPtr<ID3D12Resource>                      m_pDsvResource;
        ComPtr<ID3D12DescriptorHeap>                m_pDsvDescriptorHeap;
        D3D12_CPU_DESCRIPTOR_HANDLE                 m_dsvHandle;

    public:
        CDX12DeferredRenderig(CDX12Device* pDevice, CDX12Command* pCommand);
        ~CDX12DeferredRenderig();

        bool Initialize(UINT width, UINT height) override;
        void Begine() override;
        void End() override;

        void Rendering(CSceneBase* pScene) override;
        void DrawForSceneLighting(CSprite* pSprite) override;
        void DrawForRenderScene(CSprite* pSprite, UINT x, UINT y, UINT width, UINT height) override;
    };

} // namespace

#endif
