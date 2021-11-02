/**
 * @file CPostEffect.cpp
 * @brief
 * @author 木村優
 * @date 2021/10/03
 */

#include "CDX12PostEffect.h"

#include "GraphicsAPI/DirectX12/CDX12Device.h"
#include "GraphicsAPI/DirectX12/CDX12Command.h"
#include "GraphicsAPI/DirectX12/CDX12Renderer.h"
#include "GraphicsAPI/DirectX12/CDX12Texture.h"
#include "GraphicsAPI/DirectX12/CDX12UnorderedAccessBuffer.h"

#include "Engine46/CGameSystem.h"
#include "Engine46/CShaderManager.h"
#include "Engine46/CSprite.h"
#include "Engine46/CMesh.h"

namespace Engine46 {

    constexpr UINT THREAD_X_SIZE = 8;
    constexpr UINT THREAD_Y_SIZE = 8;
    constexpr UINT THREAD_Z_SIZE = 1;
    constexpr UINT THREAD_SIZE = THREAD_X_SIZE * THREAD_Y_SIZE;

    // コンストラクタ
    CDX12PostEffect::CDX12PostEffect(CDX12Device* pDevice, CDX12Command* pCommand) :
        pDX12Device(pDevice),
        pDX12Command(pCommand)
    {}

    // デストラクタ
    CDX12PostEffect::~CDX12PostEffect()
    {}

    // 初期化
    bool CDX12PostEffect::Initialize(CRendererBase* pRenderer, UINT width, UINT height) {

        CDX12Renderer* pDX12Renderer = dynamic_cast<CDX12Renderer*>(pRenderer);
        if (!pDX12Renderer) return false;

        UINT w = width;
        UINT h = height;
        UINT count = 0;
        for (;;) {
            if (w < 64 || h < 64) break;

            w >>= 1;
            h >>= 1;

            count++;
        }

        count *= 2;

        D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};

        dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        dhDesc.NumDescriptors = count + 1;
        dhDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        pDX12Device->CreateDescriptorHeap(m_pRtvDescriptorHeap, dhDesc);

        UINT heapSize = pDX12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        {
            D3D12_RESOURCE_DESC rDesc = {};

            rDesc.Width = width;
            rDesc.Height = height;
            rDesc.MipLevels = 1;
            rDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
            rDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            rDesc.DepthOrArraySize = 1;
            rDesc.SampleDesc = { 1 , 0 };
            rDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            rDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

            D3D12_CLEAR_VALUE clearValue = {};

            clearValue.Format = rDesc.Format;
            clearValue.Color[0] = 0.0f;
            clearValue.Color[1] = 0.0f;
            clearValue.Color[2] = 0.0f;
            clearValue.Color[3] = 1.0f;

            D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};

            rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
            rtvDesc.Format = rDesc.Format;

            pDX12Renderer->CreateRenderTexture(m_pLuminanceExtractionTexture, rDesc, clearValue);
            pDX12Renderer->CreateShaderResourceView(m_pLuminanceExtractionTexture.get());
            pDX12Renderer->CreateUnorderedAccessBufferView(m_pLuminanceExtractionTexture.get());

            m_luminanceExtractionHandle = m_pRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

            pDX12Device->CreateRenderTargetView(m_pLuminanceExtractionTexture->GetResource(), &rtvDesc, m_luminanceExtractionHandle);
        }

        {
            UINT w = width;
            UINT h = height;
            for (UINT i = 0; i < count; ++i) {
                D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_pRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
                rtvHandle.ptr += heapSize * (i + 1);

                if ((i % 2) == 0)
                {
                    w >>= 1;
                    h >>= 1;
                }

                D3D12_RESOURCE_DESC rDesc = {};

                rDesc.Width = w;
                rDesc.Height = h;
                rDesc.MipLevels = 1;
                rDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
                rDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
                rDesc.DepthOrArraySize = 1;
                rDesc.SampleDesc = { 1 , 0 };
                rDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                rDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

                D3D12_CLEAR_VALUE clearValue = {};

                clearValue.Format = rDesc.Format;
                clearValue.Color[0] = 0.0f;
                clearValue.Color[1] = 0.0f;
                clearValue.Color[2] = 0.0f;
                clearValue.Color[3] = 1.0f;

                D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};

                rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
                rtvDesc.Format = rDesc.Format;

                std::unique_ptr<CDX12Texture> pRenderTex;
                pDX12Renderer->CreateRenderTexture(pRenderTex, rDesc, clearValue);
                pDX12Renderer->CreateShaderResourceView(pRenderTex.get());
                pDX12Renderer->CreateUnorderedAccessBufferView(pRenderTex.get());

                pDX12Device->CreateRenderTargetView(pRenderTex->GetResource(), &rtvDesc, rtvHandle);

                m_pVecBlurTexture.emplace_back(std::move(pRenderTex));

                m_vecBlurHandle.emplace_back(rtvHandle);

                std::unique_ptr<CConstantBufferBase> blurCb;
                pDX12Renderer->CreateConstantBuffer(blurCb, sizeof(PostEffectCB));

                m_pVecBlurCb.emplace_back(std::move(blurCb));
            }
        }

        {
            D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};

            dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
            dhDesc.NumDescriptors = 1;
            dhDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

            pDX12Device->CreateDescriptorHeap(m_pDsvDescriptorHeap, dhDesc);

            m_dsvHandle = m_pDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

            D3D12_RESOURCE_DESC rDesc = {};

            rDesc.Width = width;
            rDesc.Height = height;
            rDesc.MipLevels = 1;
            rDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
            rDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
            rDesc.DepthOrArraySize = 1;
            rDesc.SampleDesc = { 1 , 0 };
            rDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            rDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

            D3D12_CLEAR_VALUE clearValue = {};

            clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            clearValue.DepthStencil.Depth = 1.0f;
            clearValue.DepthStencil.Stencil = 0;

            CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

            pDX12Device->CreateResource(m_pDsvResource, prop, rDesc, &clearValue);

            D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

            dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
            dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            dsvDesc.Texture2D.MipSlice = 0;
            dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

            pDX12Device->CreateDepthStencilView(m_pDsvResource.Get(), &dsvDesc, m_dsvHandle);
        }

        return true;
    }

    // ガウシアン分布
    float GaussianDistribution(const VECTOR2& pos, float rho) {
        return exp(-(pos.x * pos.x + pos.y * pos.y) / (2.0f * rho * rho));
    }

    // ブラーコンスタントバッファを更新
    void CDX12PostEffect::UpdateBlurConstantBuffer(CConstantBufferBase* pCb, UINT width, UINT height, VECTOR2& BlurDir, float m) {
        
        if (!pCb) return;
        

        const float tu = 1.0f / (float)width;
        const float tv = 1.0f / (float)height;
        const float deviation = 2.5f;

        PostEffectCB blurCb = {};

        blurCb.blurOffset[0].x = 0.0f;
        blurCb.blurOffset[0].y = 0.0f;
        blurCb.blurOffset[0].z = GaussianDistribution(VECTOR2(0.0f, 0.0f), deviation) * m;
        
        float totalWeight = blurCb.blurOffset[0].z;
        int pivot = OFFSET_MAX / 2;

        for (int i = 1; i < pivot; ++i) {
            blurCb.blurOffset[i].x = BlurDir.x * i * tu;
            blurCb.blurOffset[i].y = BlurDir.y * i * tv;
            blurCb.blurOffset[i].z = GaussianDistribution(BlurDir * (float)i, deviation) * m;
            totalWeight += blurCb.blurOffset[i].z * 2.0f;
        }

        for (int i = 0; i < pivot; ++i) {
            blurCb.blurOffset[i].z /= totalWeight;
        }

        for (int i = pivot; i < OFFSET_MAX; ++i) {
            blurCb.blurOffset[i].x = -blurCb.blurOffset[i - 7].x;
            blurCb.blurOffset[i].y = -blurCb.blurOffset[i - 7].y;
            blurCb.blurOffset[i].z = blurCb.blurOffset[i - 7].z;
        }

        blurCb.texWidth = width;
        blurCb.texHeight = height;

        pCb->Update(&blurCb);
    }

    // ブルーム結果を描画
    void CDX12PostEffect::DrawForBloom(CSprite* pSprite, CDX12Texture* pDX12Texture) {
        if (!pSprite) return;
        if (!pDX12Texture) return;

        CShaderManager* pShaderManager = CGameSystem::GetGameSystem().GetShaderManager();

        CShaderPackage* pSp = pShaderManager->CreateShaderPackage("PostEffect_Bloom.hlsl");
        if (pSp) {
            pSp->SetShader();

            pDX12Texture->Set((UINT)MyRS_Bloom::SRV_0);

            UINT index = (UINT)MyRS_Bloom::SRV_1;
            for (int i = 1; i < m_pVecBlurTexture.size(); i += 2) {
                m_pVecBlurTexture[i]->Set(index++);
            }

            pSprite->GetMesh()->Draw();
        }
    }

    // クリアカラー(コンピュートシェーダー用)
    void CDX12PostEffect::ClearColor_CS(CDX12Texture* pDX12Texture) {
        if (!pDX12Texture) return;

        UINT w = pDX12Texture->GetTextureWidth();
        UINT h = pDX12Texture->GetTextureHeight();

        pDX12Command->SetResourceBarrier(pDX12Texture->GetResource(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

        pDX12Texture->SetCompute((UINT)MyRS_ClearColor::UAV_0);

        pDX12Command->Dispatch(w / THREAD_X_SIZE, h / THREAD_Y_SIZE, THREAD_Z_SIZE);

        pDX12Command->SetResourceBarrier(pDX12Texture->GetResource(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ);
    }

    // 輝度抽出(コンピュートシェーダー用)
    void CDX12PostEffect::LuminanceExtraction_CS(CDX12Texture* pDX12InTexture, CDX12Texture* pDX12OutTexture) {
        if (!pDX12InTexture) return;
        if (!pDX12OutTexture) return;

        UINT w = pDX12OutTexture->GetTextureWidth();
        UINT h = pDX12OutTexture->GetTextureHeight();

        pDX12Command->SetResourceBarrier(pDX12OutTexture->GetResource(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

        pDX12InTexture->SetCompute((UINT)MyRS_CS_LuminanceExtraction::UAV_0);
        pDX12OutTexture->SetCompute((UINT)MyRS_CS_LuminanceExtraction::UAV_1);

        pDX12Command->Dispatch(w / THREAD_X_SIZE, h / THREAD_Y_SIZE, THREAD_Z_SIZE);

        pDX12Command->SetResourceBarrier(pDX12OutTexture->GetResource(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ);
    }

    // ポストエフェクトブラー(コンピュートシェーダー用)
    void CDX12PostEffect::PostEffectBlur_CS(CDX12Texture* pDX12Texture) {

        CShaderManager* pShaderManager = CGameSystem::GetGameSystem().GetShaderManager();

        CShaderPackage* pSp = pShaderManager->CreateShaderPackage("CS_ClearColor.hlsl");
        if (pSp) {
            pSp->SetShader();

            for (const auto& pTexture : m_pVecBlurTexture) {
                ClearColor_CS(pTexture.get());
            }

            ClearColor_CS(m_pLuminanceExtractionTexture.get());
        }

        pSp = pShaderManager->CreateShaderPackage("CS_LuminanceExtraction.hlsl");
        if (pSp) {
            pSp->SetShader();

            LuminanceExtraction_CS(pDX12Texture, m_pLuminanceExtractionTexture.get());
        }

        int size = (int)m_pVecBlurTexture.size() - 1;

        pSp = pShaderManager->CreateShaderPackage("CS_HorizonalBlur.hlsl");
        if (pSp) {
            pSp->SetShader();

            for (int i = 0; i < size; i += 2) {

                UINT w = m_pVecBlurTexture[i]->GetTextureWidth();
                UINT h = m_pVecBlurTexture[i]->GetTextureHeight();

                VECTOR2 dir = VECTOR2(1.0f, 0.0f);
                UpdateBlurConstantBuffer(m_pVecBlurCb[i].get(), w, h, dir, 2.0f);
                m_pVecBlurCb[i]->SetCompute((UINT)MyRS_CS_Blur::CBV_Blur);

                if (i == 0) {
                    Blur_CS(m_pLuminanceExtractionTexture.get(), m_pVecBlurTexture[i].get());
                }
                else {
                    Blur_CS(m_pVecBlurTexture[i - 2].get(), m_pVecBlurTexture[i].get());
                }
            }
        }

        pSp = pShaderManager->CreateShaderPackage("CS_VerticalBlur.hlsl");
        if (pSp) {
            pSp->SetShader();

            for (int i = 1; i < size; i += 2) {
                UINT w = m_pVecBlurTexture[i]->GetTextureWidth();
                UINT h = m_pVecBlurTexture[i]->GetTextureHeight();

                VECTOR2 dir = VECTOR2(0.0f, 1.0f);
                UpdateBlurConstantBuffer(m_pVecBlurCb[i].get(), w, h, dir, 2.0f);
                m_pVecBlurCb[i]->SetCompute((UINT)MyRS_CS_Blur::CBV_Blur);

                Blur_CS(m_pVecBlurTexture[i - 1].get(), m_pVecBlurTexture[i].get());
            }
        }
    }

    // ブラー(コンピュートシェーダー用)
    void CDX12PostEffect::Blur_CS(CDX12Texture* pDX12InTexture, CDX12Texture* pDX12OutTexture) {
        if (!pDX12InTexture) return;
        if (!pDX12OutTexture) return;
        
        UINT w = pDX12OutTexture->GetTextureWidth();
        UINT h = pDX12OutTexture->GetTextureHeight();

        pDX12Command->SetResourceBarrier(pDX12OutTexture->GetResource(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

        pDX12InTexture->SetCompute((UINT)MyRS_CS_Blur::UAV_0);
        pDX12OutTexture->SetCompute((UINT)MyRS_CS_Blur::UAV_1);

        pDX12Command->Dispatch(w / THREAD_X_SIZE, h / THREAD_Y_SIZE, THREAD_Z_SIZE);

        pDX12Command->SetResourceBarrier(pDX12OutTexture->GetResource(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ);
    }

    //輝度抽出描画
    void CDX12PostEffect::LuminanceExtraction(CDX12Texture* pDX12Texture, CSprite* pSprite) {

        pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

        pDX12Command->ClearDepthStencilView(m_dsvHandle, D3D12_CLEAR_FLAG_DEPTH);

        pDX12Texture->Set((UINT)MyRS_LuminanceExtraction::SRV_0);

        RenderingForPostEffect(m_pLuminanceExtractionTexture.get(), m_luminanceExtractionHandle, pSprite);

        pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
    }

    // ブラー
    void CDX12PostEffect::PostEffectBlur(CDX12Texture* pDX12Texture, CSprite* pSprite) {
        if (!pSprite) return;
        if (!pDX12Texture) return;

        CShaderManager* pShaderManager = CGameSystem::GetGameSystem().GetShaderManager();

        CShaderPackage* pSp = pShaderManager->CreateShaderPackage("LuminanceExtraction.hlsl");
        if (pSp) {
            pSp->SetShader();

            LuminanceExtraction(pDX12Texture, pSprite);
        }

        pSp = pShaderManager->CreateShaderPackage("PostEffect_Blur.hlsl");
        if (pSp) {
            pSp->SetShader();

            pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

            pDX12Command->ClearDepthStencilView(m_dsvHandle, D3D12_CLEAR_FLAG_DEPTH);

            m_pLuminanceExtractionTexture->Set((UINT)MyRS_Blur::SRV_0);

            float m = 2.0f;
            int size = (int)m_pVecBlurTexture.size() - 1;
            for (int i = 0; i < size; i += 2) {
                m *= 2;

                UINT w = m_pVecBlurTexture[i]->GetTextureWidth();
                UINT h = m_pVecBlurTexture[i]->GetTextureHeight();

                pDX12Command->SetViewPort(0, 0, w, h);
                
                VECTOR2 dir = VECTOR2(1.0f, 0.0f);
                UpdateBlurConstantBuffer(m_pVecBlurCb[i].get(), w, h, dir, m);
                m_pVecBlurCb[i]->Set((UINT)MyRS_Blur::CBV_Blur);

                RenderingForPostEffect(m_pVecBlurTexture[i].get(), m_vecBlurHandle[i], pSprite);

                dir = VECTOR2(0.0f, 1.0f);
                UpdateBlurConstantBuffer(m_pVecBlurCb[i + 1].get(), w, h, dir, m);
                m_pVecBlurCb[i + 1]->Set((UINT)MyRS_Blur::CBV_Blur);

                m_pVecBlurTexture[i]->Set((UINT)MyRS_Blur::SRV_0);
                RenderingForPostEffect(m_pVecBlurTexture[i + 1].get(), m_vecBlurHandle[i + 1], pSprite);
            }

            pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
        }
    }

    // ブラー描画
    void CDX12PostEffect::RenderingForPostEffect(CDX12Texture* pDX12Texture, D3D12_CPU_DESCRIPTOR_HANDLE handle, CSprite* pSprite) {

        pDX12Command->SetResourceBarrier(pDX12Texture->GetResource(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);

        pDX12Command->ClearRenderTargetView(handle);
        pDX12Command->SetRenderTargetView(&handle, &m_dsvHandle);

        pSprite->GetMesh()->Draw();

        pDX12Command->SetResourceBarrier(pDX12Texture->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
    }

} // namepsace
