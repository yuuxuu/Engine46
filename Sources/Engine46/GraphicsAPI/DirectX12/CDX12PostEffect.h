/**
 * @file CDX12PostEffect.h
 * @brief
 * @author 木村優
 * @date 2021/10/03
 */

#pragma once

#ifndef _CPOST_EFFECT_H
#define _CPOST_EFFECT_H

#include "../IPostEffect.h"

#include "../math.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

    // 前方宣言
    class CDX12Device;
    class CDX12Command;
    class CDX12Texture;
    class CDX12UnorderedAccessBuffer;
    class CConstantBufferBase;
    class CSprite;

    class CDX12PostEffect : public IPostEffect {
    private:
        CDX12Device*                                        pDX12Device;
        CDX12Command*                                       pDX12Command;

        std::unique_ptr<CDX12Texture>                       m_pBloomTexture;
        D3D12_CPU_DESCRIPTOR_HANDLE                         m_bloomHandle;

        std::unique_ptr<CDX12Texture>                       m_pLuminanceExtractionTexture;
        D3D12_CPU_DESCRIPTOR_HANDLE                         m_luminanceExtractionHandle;

        std::unique_ptr<CDX12Texture>                       m_pToneMapTexture;
        D3D12_CPU_DESCRIPTOR_HANDLE                         m_toneMapHandle;

        std::vector<std::unique_ptr<CDX12Texture>>          m_pVecBlurTexture;
        std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>            m_vecBlurHandle;

        std::vector<std::unique_ptr<CConstantBufferBase>>   m_pVecBlurCb;

        ComPtr<ID3D12DescriptorHeap>                        m_pRtvDescriptorHeap;

        ComPtr<ID3D12Resource>                              m_pDsvResource;
        ComPtr<ID3D12DescriptorHeap>                        m_pDsvDescriptorHeap;
        D3D12_CPU_DESCRIPTOR_HANDLE                         m_dsvHandle;

    public:
        CDX12PostEffect(CDX12Device* pDevice, CDX12Command* pCommand);
        ~CDX12PostEffect();

        bool Initialize(CRendererBase* pRenderer, UINT width, UINT height) override;

        void UpdateBlurConstantBuffer(CConstantBufferBase* pCb, UINT width, UINT height, VECTOR2& BlurDir, float m);

        void ClearColor_CS(CDX12Texture* pDX12Texture);
        void LuminanceExtraction_CS(CDX12Texture* pDX12InTexture, CDX12Texture* pDX12OutTexture);
        void ToneMap_CS(CDX12Texture* pDX12Texture);
        void PostEffectBlur_CS(CDX12Texture* pDX12Texture);
        void Blur_CS(CDX12Texture* pDX12InTexture, CDX12Texture* pDX12OutTexture);

        void LuminanceExtraction(CDX12Texture* pDX12Texture, CSprite* pSprite);
        void PostEffectBlur(CDX12Texture* pDX12Texture, CSprite* pSprite);
        void PostEffectBloom(CDX12Texture* pDX12Texture, CSprite* pSprite);
        void RenderingForPostEffect(CDX12Texture* pDX12Texture, D3D12_CPU_DESCRIPTOR_HANDLE handle, CSprite* pSprite);

        CDX12Texture* GetLuminanceExtractionTexture() const { return m_pLuminanceExtractionTexture.get(); }
        CDX12Texture* GetBloomTexture() const { return m_pBloomTexture.get(); }
        CDX12Texture* GetToneMapTexture() const { return m_pToneMapTexture.get(); }
    };

} // namespace

#endif
