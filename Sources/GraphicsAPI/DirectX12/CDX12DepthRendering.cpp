/**
 * @file CDX12DepthRendering.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/23
 */

#include "CDX12DepthRendering.h"
#include "CDX12Device.h"
#include "CDX12Command.h"
#include "CDX12Renderer.h"
#include "CDX12Texture.h"

#include "Engine46/CRendererSystem.h"

namespace Engine46 {

    // コンストラクタ
    CDX12DepthRendering::CDX12DepthRendering(CDX12Device* pDevice, CDX12Command* pCommand) :
        pDX12Device(pDevice),
        pDX12Command(pCommand)
    {}

    // デストラクタ
    CDX12DepthRendering::~CDX12DepthRendering()
    {}

    // 初期化
    bool CDX12DepthRendering::Initialize(UINT width, UINT height) {

        CDX12Renderer* pRenderer = dynamic_cast<CDX12Renderer*>(CRendererSystem::GetRendererSystem().GetRenderer());
        if (!pRenderer) return false;

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

            D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

            dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
            dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            dsvDesc.Texture2D.MipSlice = 0;
            dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

            std::unique_ptr<CDX12Texture> depthTexture;
            pRenderer->CreateRenderTexture(depthTexture, rDesc, clearValue, TextureType::Depth);

            pDX12Device->CreateDepthStencilView(depthTexture->GetResource(), &dsvDesc, m_dsvHandle);

            pDX12DepthTexture = depthTexture.get();
            m_pRenderTex = std::move(depthTexture);
        }

        return true;
    }

    // 描画開始
    void CDX12DepthRendering::Begine() {
        pDX12Command->SetResourceBarrier(pDX12DepthTexture->GetResource(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

        pDX12Command->ClearDepthStencilView(m_dsvHandle, D3D12_CLEAR_FLAG_DEPTH);

        pDX12Command->SetRenderTargetView(nullptr, &m_dsvHandle);
    }

    // 描画終了
    void CDX12DepthRendering::End() {
        pDX12Command->SetResourceBarrier(pDX12DepthTexture->GetResource(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
    }

    // シーン描画
    void CDX12DepthRendering::Rendering(CSceneBase* pScene) {

        Begine();

        pScene->Draw();

        End();
    }

    // 描画したシーン描画
    void CDX12DepthRendering::DrawForRenderScene(CSprite* pSprite, UINT x, UINT y, UINT width, UINT height) {

        if (!pSprite) return;

        pDX12Command->SetViewPort(x, y, width, height);

        pSprite->SetTexture(pDX12DepthTexture);
        pSprite->Draw();
    }

} // namespace
