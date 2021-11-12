/**
 * @file CDX12DeferredRendering.cpp
 * @brief
 * @author 木村優
 * @date 2021/09/16

 */

#include "CDX12DeferredRenderig.h"
#include "CDX12Device.h"
#include "CDX12Command.h"
#include "CDX12Renderer.h"
#include "CDX12Texture.h"
#include "CDX12UnorderedAccessBuffer.h"

#include "../CRendererSystem.h"
#include "../CGameSystem.h"
#include "../CShaderManager.h"
#include "../CMaterial.h"
#include "../CMesh.h"
#include "../CLight.h"

namespace Engine46 {

    // コンストラクタ
    CDX12DeferredRenderig::CDX12DeferredRenderig(CDX12Device* pDevice, CDX12Command* pCommand) :
        pDX12Device(pDevice),
        pDX12Command(pCommand)
    {}

    // デストラクタ
    CDX12DeferredRenderig::~CDX12DeferredRenderig()
    {}

    // 初期化
    bool CDX12DeferredRenderig::Initialize(UINT width, UINT height) {

        CDX12Renderer* pRenderer = dynamic_cast<CDX12Renderer*>(CRendererSystem::GetRendererSystem().GetRenderer());
        if (!pRenderer) return false;

        D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};

        dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        dhDesc.NumDescriptors = RENDER_TARGET_SIZE + 1;
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

            pRenderer->CreateRenderTexture(m_pRenderTex, rDesc, clearValue);
            pRenderer->CreateShaderResourceView(m_pRenderTex.get());
            pRenderer->CreateUnorderedAccessBufferView(m_pRenderTex.get());

            m_rtvHandle = m_pRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

            pDX12Device->CreateRenderTargetView(m_pRenderTex->GetResource(), &rtvDesc, m_rtvHandle);
        }

        {
            for (UINT i = 0; i < RENDER_TARGET_SIZE; ++i) {
                D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_pRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
                rtvHandle.ptr += heapSize * (i + 1);

                D3D12_RESOURCE_DESC rDesc = {};

                rDesc.Width = width;
                rDesc.Height = height;
                rDesc.MipLevels = 1;
                rDesc.Format = RENDER_TARGET_FORMATS[i];
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

                pDX12Device->CreateRenderTargetView(pRenderTex->GetResource(), &rtvDesc, rtvHandle);

                m_pVecRenderTex.emplace_back(std::move(pRenderTex));

                m_vecRtvHandle.emplace_back(rtvHandle);
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

    // 描画開始
    void CDX12DeferredRenderig::Begine() {
        for (const auto& pRenderTex : m_pVecRenderTex) {
            pDX12Command->SetResourceBarrier(pRenderTex->GetResource(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
        }
        pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

        for (const auto& rtvHandle : m_vecRtvHandle) {
            pDX12Command->ClearRenderTargetView(rtvHandle);
        }
        pDX12Command->ClearDepthStencilView(m_dsvHandle, D3D12_CLEAR_FLAG_DEPTH);

        pDX12Command->SetRenderTargetViews(m_vecRtvHandle, &m_dsvHandle);
    }

    // 描画終了
    void CDX12DeferredRenderig::End() {
        for (const auto& pRenderTex : m_pVecRenderTex) {
            pDX12Command->SetResourceBarrier(pRenderTex->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
        }
        pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
    }

    // シーン描画
    void CDX12DeferredRenderig::Rendering(CSceneBase* pScene) {

        if (!pScene) return;

        Begine();

        CShaderManager* pShaderManager = CGameSystem::GetGameSystem().GetShaderManager();

        CShaderPackage* pSp = pShaderManager->CreateShaderPackage("GBuffer.hlsl");
        if (pSp) {
            pSp->SetShader();
            pSp->SetSceneConstantBufferToShader((UINT)CB_TYPE::CAMERA);

            std::vector<CSprite*> vecSprites = pScene->GetSpritesFromScene();
            for (const auto& pSprite : vecSprites) {
                Matrix matW = pSprite->GetWorldMatrix();
                matW.dx_m = DirectX::XMMatrixTranspose(matW.dx_m);

                worldCB cb = {
                    matW,
                };
                pSprite->UpdateWorldConstantBuffer(&cb);

                pSprite->GetMesh()->Set();

                pSprite->GetMesh()->Draw();
            }

            std::vector<CActorBase*> vecBoxs = pScene->GetBoxsFromScene();
            for (const auto& pBox : vecBoxs) {
                Matrix matW = pBox->GetWorldMatrix();
                matW.dx_m = DirectX::XMMatrixTranspose(matW.dx_m);

                worldCB cb = {
                    matW,
                };
                pBox->UpdateWorldConstantBuffer(&cb);

                pBox->GetMesh()->Set();

                pBox->GetMesh()->Draw();
            }
        }

        std::vector<CLight*> vecLight = pScene->GetLightsFromScene();
        for (const auto& pLight : vecLight) {
            pLight->Draw();
        }

        End();
    }

    // 描画したシーンをライティング描画
    void CDX12DeferredRenderig::RenderingForSceneLighting(CSprite* pSprite) {

        if (!pSprite) return;

        CShaderManager* pShaderManager = CGameSystem::GetGameSystem().GetShaderManager();

        CShaderPackage* pSp = pShaderManager->CreateShaderPackage("GBuffer_Lighting.hlsl");
        if (pSp) {
            pDX12Command->SetResourceBarrier(m_pRenderTex->GetResource(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
            pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

            pDX12Command->ClearDepthStencilView(m_dsvHandle, D3D12_CLEAR_FLAG_DEPTH);
            pDX12Command->ClearRenderTargetView(m_rtvHandle);
            pDX12Command->SetRenderTargetView(&m_rtvHandle, &m_dsvHandle);

            pSp->SetShader();
            pSp->SetSceneConstantBufferToShader((UINT)MyRS_GBuffer_Ligthing::CBV_CAMERA);

            UINT index = (UINT)MyRS_GBuffer_Ligthing::SRV_0;
            for (const auto& pTexture : m_pVecRenderTex) {
                pTexture->Set(index++);
            }

            pSprite->GetMesh()->Draw();

            pDX12Command->SetResourceBarrier(m_pRenderTex->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
            pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
        }
    }

    // ポストエフェクト準備描画
    void CDX12DeferredRenderig::RenderingForPostEffect(CSceneBase* pScene) {

        pDX12Command->SetResourceBarrier(m_pRenderTex->GetResource(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
        pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

        pDX12Command->ClearDepthStencilView(m_dsvHandle, D3D12_CLEAR_FLAG_DEPTH);
        pDX12Command->ClearRenderTargetView(m_rtvHandle);
        pDX12Command->SetRenderTargetView(&m_rtvHandle, &m_dsvHandle);

        std::vector<CLight*> vecLight = pScene->GetLightsFromScene();
        for (const auto& pLight : vecLight) {
            pLight->Draw();
        }

        pDX12Command->SetResourceBarrier(m_pRenderTex->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
        pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
    }

    // 描画したシーン描画
    void CDX12DeferredRenderig::DrawForRenderScene(CSprite* pSprite, UINT x, UINT y, UINT width, UINT height) {

        if (!pSprite) return;

        for (const auto& pRenderTex : m_pVecRenderTex) {
            pDX12Command->SetViewPort(x, y, width, height);

            CMeshBase* pMesh = pSprite->GetMesh();
            if (pMesh) {
                CMaterialBase* pMaterial = pMesh->GetMaterial();
                pMaterial->SetTexture(pRenderTex.get());
            }

            pSprite->Draw();

            x += width;
        }
    }

    // MRTレンダーテクスチャを取得
    CDX12Texture* CDX12DeferredRenderig::GetRenderTexture(DXGI_FORMAT format) {

        for (int i = 0; i < RENDER_TARGET_SIZE; ++i) {
            if (RENDER_TARGET_FORMATS[i] == format) {
                return m_pVecRenderTex[i].get();
            }
        }

        return nullptr;
    }

} // namespace
