/**
 * @file CDX12ForwardRendering.cpp
 * @brief
 * @author 木村優
 * @date 2021/09/16
 */

#include "CDX12ForwardRendering.h"
#include "CDX12Device.h"
#include "CDX12Command.h"
#include "CDX12Texture.h"
#include "CDX12Renderer.h"

#include "Engine46/CRendererSystem.h"
#include "Engine46/CLight.h"
#include "Engine46/CMaterial.h"
#include "Engine46/CMesh.h"

namespace Engine46 {

    // コンストラクタ
    CDX12ForwardRendering::CDX12ForwardRendering(CDX12Device* pDevice, CDX12Command* pCommand) :
        pDX12Device(pDevice),
        pDX12Command(pCommand)
    {}

    // デストラクタ
    CDX12ForwardRendering::~CDX12ForwardRendering()
    {}

    // 初期化
    bool CDX12ForwardRendering::Initialize(UINT width, UINT height) {

        CDX12Renderer* pRenderer = dynamic_cast<CDX12Renderer*>(CRendererSystem::GetRendererSystem().GetRenderer());
        if (!pRenderer) return false;

        {
            D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};

            dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            dhDesc.NumDescriptors = 1;
            dhDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

            pDX12Device->CreateDescriptorHeap(m_pRtvDescriptorHeap, dhDesc);

            m_rtvHandle = m_pRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

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

            std::unique_ptr<CDX12Texture> pRenderTex;
            pRenderer->CreateRenderTexture(pRenderTex, rDesc, clearValue);
            pRenderer->CreateShaderResourceView(pRenderTex.get());
            pRenderer->CreateUnorderedAccessBufferView(pRenderTex.get());

            pDX12Device->CreateRenderTargetView(pRenderTex->GetResource(), &rtvDesc, m_rtvHandle);

            pDX12RenderTexture = pRenderTex.get();
            m_pRenderTex = std::move(pRenderTex);
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

    // レンダリング開始
    void CDX12ForwardRendering::Begine() {
        pDX12Command->SetResourceBarrier(pDX12RenderTexture->GetResource(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
        pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

        pDX12Command->ClearRenderTargetView(m_rtvHandle);
        pDX12Command->ClearDepthStencilView(m_dsvHandle, D3D12_CLEAR_FLAG_DEPTH);
        pDX12Command->SetRenderTargetView(&m_rtvHandle, &m_dsvHandle);
    }

    // レンダリング終了
    void CDX12ForwardRendering::End() {
        pDX12Command->SetResourceBarrier(pDX12RenderTexture->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
        pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
    }

    // シーン描画
    void CDX12ForwardRendering::Rendering(CSceneBase* pScene) {

        Begine();

        pScene->Draw();

        End();
    }

    // 描画したシーン描画
    void CDX12ForwardRendering::RenderingForRenderScene(CSprite* pSprite, UINT x, UINT y, UINT width, UINT height) {

        if (!pSprite) return;

        pDX12Command->SetViewPort(x, y, width, height);

        CMeshBase* pMesh = pSprite->GetMesh();
        if (pMesh) {
            CMaterialBase* pMaterial = pMesh->GetMaterial();
            pMaterial->SetTexture(pDX12RenderTexture);
        }

        pSprite->Draw();
    }

} // namespace
