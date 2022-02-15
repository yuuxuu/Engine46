/**
 * @file CDX12TiledForwardRendering.cpp
 * @brief
 * @author 木村優
 * @date 2022/01/08
 */

#include "CDX12TiledForwardRendering.h"
#include "CDX12Device.h"
#include "CDX12Command.h"
#include "CDX12Texture.h"
#include "CDX12Renderer.h"
#include "CDX12UnorderedAccessBuffer.h"

#include "../CGameSystem.h"
#include "../CRendererSystem.h"
#include "../CShaderManager.h"
#include "../CMaterial.h"
#include "../CMesh.h"
#include "../CModelMesh.h"
#include "../CLight.h"

namespace Engine46{

    constexpr UINT TILE_SIZE_X = 16;
    constexpr UINT TILE_SIZE_Y = 16;
    constexpr UINT TILE_SIZE = TILE_SIZE_X * TILE_SIZE_Y;

    // コンストラクタ
    CDX12TiledForwardRendering::CDX12TiledForwardRendering(CDX12Device* pDevice, CDX12Command* pCommand) :
        pDX12Device(pDevice),
        pDX12Command(pCommand),
        m_numLight(0)
    {}

    // デストラクタ
    CDX12TiledForwardRendering::~CDX12TiledForwardRendering()
    {}

    // 初期化
    bool CDX12TiledForwardRendering::Initialize(UINT width, UINT height) {

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

    // 描画開始
    void CDX12TiledForwardRendering::Begine() {
        pDX12Command->SetResourceBarrier(pDX12RenderTexture->GetResource(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
        pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

        pDX12Command->ClearRenderTargetView(m_rtvHandle);
        pDX12Command->ClearDepthStencilView(m_dsvHandle, D3D12_CLEAR_FLAG_DEPTH);
        pDX12Command->SetRenderTargetView(&m_rtvHandle, &m_dsvHandle);
    }

    // 描画終了
    void CDX12TiledForwardRendering::End() {
        pDX12Command->SetResourceBarrier(pDX12RenderTexture->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
        pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
    }

    // シーン描画
    void CDX12TiledForwardRendering::Rendering(CSceneBase* pScene) {

        Begine();

        CActorBase* pSkyDome = pScene->GetSkyDomeFromScene();
        if (pSkyDome) {
            CShaderPackage* pSp = pSkyDome->GetShaderPackage();
            if (pSp) {
                pSp->SetShader();

                CRendererBase* pRenderer = CRendererSystem::GetRendererSystem().GetRenderer();
                if (pRenderer) {
                    pRenderer->SetSceneConstantBuffers((UINT)CB_TYPE::CAMERA);
                }

                CConstantBufferBase* pCb = pSkyDome->GetWorldConstantBuffer();
                if (pCb) {
                    Matrix matW = pSkyDome->GetWorldMatrix();
                    matW.dx_m = DirectX::XMMatrixTranspose(matW.dx_m);

                    worldCB cb = {
                        matW,
                    };
                    pSkyDome->UpdateWorldConstantBuffer(&cb);

                    CModelMesh* pModelMesh = pSkyDome->GetModelMesh();
                    if (pModelMesh) {
                        std::vector<CMeshBase*> vecMesh = pModelMesh->GetVecMesh();
                        for (const auto& mesh : vecMesh) {
                            mesh->Set();
                            CMaterialBase* pMaterial = mesh->GetMaterial();
                            if (pMaterial) {
                                pMaterial->SetTexture((UINT)MyRS_Model::SRV_Diffuse);
                            }
                            mesh->Draw();
                        }
                    }
                }
            }
        }

        CShaderManager* pShaderManager = CGameSystem::GetGameSystem().GetShaderManager();

        CShaderPackage* pSp = pShaderManager->CreateShaderPackage("Model_LightingOfLightCulling.hlsl");
        if (pSp) {
            pSp->SetShader();

            m_pLightIndexUab->Set((UINT)MyRS_ModelLighting_Of_LightCulling::UAV_0);

            CRendererBase* pRenderer = CRendererSystem::GetRendererSystem().GetRenderer();
            if (pRenderer) {
                pRenderer->SetSceneConstantBuffers((UINT)MyRS_ModelLighting_Of_LightCulling::CBV_Camera);
                pRenderer->SetCubeTexture((UINT)MyRS_ModelLighting_Of_LightCulling::SRV_Cube);
                pRenderer->SetScreenParamCb((UINT)MyRS_ModelLighting_Of_LightCulling::CBV_ScreenParam, false);
            }

            std::vector<CActorBase*> vecActors = pScene->GetActorsFromScene();
            for (const auto& pActor : vecActors) {
                Matrix matW = pActor->GetWorldMatrix();
                matW.dx_m = DirectX::XMMatrixTranspose(matW.dx_m);

                worldCB cb = {
                    matW,
                };
                pActor->UpdateWorldConstantBuffer(&cb);

                CMeshBase* pMesh = pActor->GetMesh();
                if (pMesh) {
                    pMesh->Set();
                    CMaterialBase* pMaterial = pMesh->GetMaterial();
                    if (pMaterial) {
                        pMaterial->SetTexture((UINT)MyRS_ModelLighting_Of_LightCulling::SRV_Diffuse);
                    }
                    pMesh->Draw();
                }
                else {
                    CModelMesh* pModelMesh = pActor->GetModelMesh();
                    if (pModelMesh) {
                        std::vector<CMeshBase*> vecMesh = pModelMesh->GetVecMesh();
                        for (const auto& mesh : vecMesh) {
                            mesh->Set();
                            std::vector<CMaterialBase*> vecMaterial = mesh->GetVecMaterial();
                            if (!vecMaterial.empty()) {
                                for (const auto& material : vecMaterial) {
                                    if (material) {
                                        material->SetTexture((UINT)MyRS_ModelLighting_Of_LightCulling::SRV_Diffuse);
                                    }
                                }
                            }
                            else {
                                CMaterialBase* pMaterial = mesh->GetMaterial();
                                if (pMaterial) {
                                    pMaterial->SetTexture((UINT)MyRS_ModelLighting_Of_LightCulling::SRV_Diffuse);
                                }
                            }
                            mesh->Draw();
                        }
                    }
                }
            }
        }

        std::vector<CLight*> vecLight = pScene->GetLightsFromScene();
        for (const auto& light : vecLight) {
            light->Draw();
        }

        End();
    }

    // 描画したシーン描画
    void CDX12TiledForwardRendering::RenderingForRenderScene(CSprite* pSprite, UINT x, UINT y, UINT width, UINT height) {

        if (!pSprite) return;

        pDX12Command->SetViewPort(x, y, width, height);

        CMeshBase* pMesh = pSprite->GetMesh();
        if (pMesh) {
            CMaterialBase* pMaterial = pMesh->GetMaterial();
            pMaterial->SetTexture(pDX12RenderTexture);
        }

        pSprite->Draw();
    }

    // ライトインデックスを更新
    void CDX12TiledForwardRendering::UpdateLightIndexUab(const int numLight) {
        if (m_numLight == numLight) return;

        m_numLight = numLight;

        UINT width = pDX12RenderTexture->GetTextureWidth();
        UINT height = pDX12RenderTexture->GetTextureHeight();
        UINT bufSize = (width / TILE_SIZE_X) * (height / TILE_SIZE_Y) * m_numLight;

        if (!m_pLightIndexUab) {
            CDX12Renderer* pRenderer = dynamic_cast<CDX12Renderer*>(CRendererSystem::GetRendererSystem().GetRenderer());
            if (!pRenderer) return;

            pRenderer->CreateUnorderedAccessBuffer(m_pLightIndexUab, sizeof(UINT), bufSize);
        }
        else {
            m_pLightIndexUab->CreateUnorderedAccessBuffer(sizeof(UINT), bufSize);
        }

    }

    // ライトカリング
    void CDX12TiledForwardRendering::LightCulling_CS(CDX12Texture* pDX12DepthTexture) {
        if (!m_pLightIndexUab) return;

        CShaderManager* pShaderManger = CGameSystem::GetGameSystem().GetShaderManager();

        CShaderPackage* pSp = pShaderManger->CreateShaderPackage("CS_LightCulling.hlsl");
        if (pSp) {
            pSp->SetShader();

            CRendererBase* pRenderer = CRendererSystem::GetRendererSystem().GetRenderer();
            if (pRenderer) {
                pRenderer->SetCameraCb((UINT)MyRS_CS_LightCulling::CBV_Camera, true);
                pRenderer->SetPointLightCb((UINT)MyRS_CS_LightCulling::CBV_PointLight, true);
                pRenderer->SetScreenParamCb((UINT)MyRS_CS_LightCulling::CBV_ScreenParam, true);
            }

            UINT width = pDX12RenderTexture->GetTextureWidth();
            UINT height = pDX12RenderTexture->GetTextureHeight();

            pDX12DepthTexture->SetCompute((UINT)MyRS_CS_LightCulling::SRV_Depth);

            m_pLightIndexUab->SetCompute((UINT)MyRS_CS_LightCulling::UAV_0);

            m_pLightIndexUab->Dispatch(width / TILE_SIZE_X, height / TILE_SIZE_Y, 1);
        }
    }

} // namespace
