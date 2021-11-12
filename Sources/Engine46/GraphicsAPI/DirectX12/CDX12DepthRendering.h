/**
 * @file CDX12DepthRendering.h
 * @brief
 * @author 木村優
 * @date 2021/05/23
 */

#pragma once

#ifndef _CDX12_DEPTH_RENDERING_H_
#define _CDX12_DEPTH_RENDERING_H_

#include "../CRendering.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

    // 前方宣言
    class CDX12Device;
    class CDX12Command;
    class CDX12Texture;

    class CDX12DepthRendering : public CRenderingBase {
    private:
        CDX12Device* pDX12Device;
        CDX12Command* pDX12Command;

        CDX12Texture* pDX12DepthTexture;

        ComPtr<ID3D12DescriptorHeap>    m_pDsvDescriptorHeap;
        D3D12_CPU_DESCRIPTOR_HANDLE     m_dsvHandle;

    public:
        CDX12DepthRendering(CDX12Device* pDevice, CDX12Command* pCommnad);
        ~CDX12DepthRendering();

        bool Initialize(UINT width, UINT height) override;
        void Begine() override;
        void End() override;

        void Rendering(CSceneBase* pScene);
        void DrawForRenderScene(CSprite* pSprite, UINT x, UINT y, UINT width, UINT height) override;
    };

} // namespace 

#endif

