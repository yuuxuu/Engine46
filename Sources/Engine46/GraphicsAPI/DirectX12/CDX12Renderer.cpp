/**
 * @file CDX12Renderer.height
 * @brief
 * @author 木村優
 * @date 2021/08/19
 */

#include "CDX12Renderer.h"
#include "CDX12Device.h"
#include "CDX12Command.h"
#include "CDX12ConstantBuffer.h"
#include "CDX12UnorderedAccessBuffer.h"
#include "CDX12Mesh.h"
#include "CDX12Texture.h"
#include "CDX12ShaderPackage.h"
#include "CDX12ForwardRendering.h"
#include "CDX12DepthRendering.h"
#include "CDX12DeferredRenderig.h"
#include "CDX12TiledForwardRendering.h"

#include "../CTextureManager.h"
#include "../CGameSystem.h"
#include "../CFileSystem.h"
#include "../CLight.h"
#include "../CPointLight.h"
#include "../CCamera.h"
#include "../CMaterial.h"

namespace Engine46 {

    constexpr UINT DESCRIPTORHEAP_MAX = STATIC_MAX + 3000;

    // コンストラクタ
    CDX12Renderer::CDX12Renderer() :
        m_descriptorHeapOffsetIndex(0)
    {}

    // デストラクタ
    CDX12Renderer::~CDX12Renderer()
    {}

    // 初期化
    bool CDX12Renderer::Initialize(HWND hwnd, UINT width, UINT height) {

        ComPtr<ID3D12CommandQueue> pCommandQueue;

        m_pDX12Device = std::make_unique<CDX12Device>();
        if (!m_pDX12Device->Initialize(pCommandQueue, hwnd, width, height)) return false;

        m_pDX12Command = std::make_unique<CDX12Command>(pCommandQueue);
        if (!m_pDX12Command->Initialize(m_pDX12Device.get())) return false;

        {
            D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};

            dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            dhDesc.NumDescriptors = 2;
            dhDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

            m_pDX12Device->CreateDescriptorHeap(m_pRtvDescriptorHeap, dhDesc);

            m_rtvHandle[0] = m_pRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
            UINT size = m_pDX12Device->GetDescriptorHandleIncrementSize(dhDesc.Type);
            m_rtvHandle[1].ptr = m_rtvHandle[0].ptr + size;

            m_pDX12Device->GetBackBuffer(m_pRtvResource[0], 0);
            m_pDX12Device->CreateRenderTargetView(m_pRtvResource[0].Get(), nullptr, m_rtvHandle[0]);

            m_pDX12Device->GetBackBuffer(m_pRtvResource[1], 1);
            m_pDX12Device->CreateRenderTargetView(m_pRtvResource[1].Get(), nullptr, m_rtvHandle[1]);
        }

        {
            D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};

            dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
            dhDesc.NumDescriptors = 1;
            dhDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

            m_pDX12Device->CreateDescriptorHeap(m_pDsvDescriptorHeap, dhDesc);

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

            m_pDX12Device->CreateResource(m_pDsvResource, prop, rDesc, &clearValue);

            D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

            dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
            dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            dsvDesc.Texture2D.MipSlice = 0;
            dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

            m_pDX12Device->CreateDepthStencilView(m_pDsvResource.Get(), &dsvDesc, m_dsvHandle);
        }

        {
            D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};

            dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            dhDesc.NumDescriptors = DESCRIPTORHEAP_MAX;
            dhDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

            m_pDX12Device->CreateDescriptorHeap(m_pCbDescriptorHeap, dhDesc);

            CreateConstantBuffer(m_pCameraCB, sizeof(CameraCB));
            CreateConstantBuffer(m_pDirectionalLightCB, sizeof(DirectionalLightCB));
            CreateConstantBuffer(m_pPointLightCB, sizeof(PointLightCB));
            CreateConstantBuffer(m_pSpotLightCB, sizeof(SpotLightCB));
            CreateConstantBuffer(m_pScreenParamCB, sizeof(ScreenParamCB));
        }

        //m_pForwardRendering = std::make_unique<CDX12ForwardRendering>(m_pDX12Device.get(), m_pDX12Command.get());
        //if (!m_pForwardRendering->Initialize(width, height)) return false;

        m_pTiledForwardRendering = std::make_unique<CDX12TiledForwardRendering>(m_pDX12Device.get(), m_pDX12Command.get());
        if (!m_pTiledForwardRendering->Initialize(width, height)) return false;

        //m_pDeferredRendering = std::make_unique<CDX12DeferredRenderig>(m_pDX12Device.get(), m_pDX12Command.get());
        //if (!m_pDeferredRendering->Initialize(width, height)) return false;

        m_pDepthRendring = std::make_unique<CDX12DepthRendering>(m_pDX12Device.get(), m_pDX12Command.get());
        if (!m_pDepthRendring->Initialize(width, height)) return false;

        m_pDX12PostEffect = std::make_unique<CDX12PostEffect>(m_pDX12Device.get(), m_pDX12Command.get());
        if (!m_pDX12PostEffect->Initialize(this, width, height)) return false;

        m_pDX12Command->CloseCommandList();

        m_windowRect = RECT(width, height);

        return true;
    }

    // 終了
    void CDX12Renderer::Finalize() {

    }

    // 描画準備開始
    void CDX12Renderer::Begine(CSceneBase* pScene) {
        if (!m_pRenderSprite) {
            m_pRenderSprite = std::make_unique<CSprite>("RenderSprite");

            std::unique_ptr<CConstantBufferBase> worldConstantBuffer;
            CreateConstantBuffer(worldConstantBuffer, sizeof(worldCB));
            m_pRenderSprite->SetWorldConstantBuffer(worldConstantBuffer);

            m_pRenderSprite->SetMesh("RenderSpriteMesh");

            CMeshBase* pMesh = m_pRenderSprite->GetMesh();
            if (pMesh) {
                pMesh->SetMaterial("RenderSpriteMaterial");

                pMesh->CreateSpriteMesh();
            }
            m_pRenderSprite->SetShaderPackage("Sprite.hlsl");
        }

        if(!pCubeTexture){
            CTextureManager* textureManager = CGameSystem::GetGameSystem().GetTextureManager();
            pCubeTexture = textureManager->CreateTexture("Snow_Bg.jpg");
            if (pCubeTexture) {
                pCubeTexture->CreateCubeTexture();

                CreateCubeTextureShaderResourceView(dynamic_cast<CDX12Texture*>(pCubeTexture));
            }
        }

        ScreenParamCB screenParamCb = {
            m_windowRect.w,
            m_windowRect.h,
            Z_NEAR,
            Z_FAR,
        };

        m_pScreenParamCB->Update(&screenParamCb);

        if (pScene) {
            CCamera* pCamera = pScene->GetCameraFromScene();
            if (pCamera) {

                Matrix matVP = pCamera->GetViewProjectionMatrix();
                matVP.dx_m = DirectX::XMMatrixTranspose(matVP.dx_m);

                Matrix matView = pCamera->GetViewMatrix();
                matView.dx_m = DirectX::XMMatrixTranspose(matView.dx_m);

                Matrix matProj = pCamera->GetProjectionMatrix();
                matProj.dx_m = DirectX::XMMatrixTranspose(matProj.dx_m);

                Matrix invMatProj = pCamera->GetInvProjectionMatrix();
                //invMatProj.dx_m = DirectX::XMMatrixTranspose(invMatProj.dx_m);

                CameraCB cb = {
                    matVP,
                    pCamera->GetPos(),
                    0.0f,
                    matView,
                    matProj,
                    invMatProj,
                };

                m_pCameraCB->Update(&cb);
            }

            std::vector<CLight*> pLights = pScene->GetLightsFromScene();
            if (!pLights.empty()) {

                DirectionalLightCB directionalLightCb = {};
                PointLightCB pointLightCb = {};
                SpotLightCB spotLightCb = {};

                for (const auto light : pLights)
                {
                    CPointLight* pLight = nullptr;

                    switch (light->GetLightType())
                    {
                    case LightType::Directional:
                        directionalLightCb.pos = light->GetPos();
                        directionalLightCb.diffuse = light->GetLightDiffuse();
                        directionalLightCb.specular = light->GetLightSpecular();
                        break;
                    case LightType::Point:
                        pLight = dynamic_cast<CPointLight*>(light);
                        if (!pLight) return;

                        pointLightCb.pointLights[pointLightCb.numPointLight].pos = pLight->GetPos();
                        pointLightCb.pointLights[pointLightCb.numPointLight].diffuse = pLight->GetLightDiffuse();
                        pointLightCb.pointLights[pointLightCb.numPointLight].specular = pLight->GetLightSpecular();
                        pointLightCb.pointLights[pointLightCb.numPointLight].attenuation = pLight->GetLightAttenuation();
                        pointLightCb.pointLights[pointLightCb.numPointLight].radius = pLight->GetRadius();

                        pointLightCb.numPointLight++;
                        break;
                    case LightType::Spot:
                        spotLightCb.spotLights[spotLightCb.numSpotLight].pos = light->GetPos();
                        spotLightCb.spotLights[spotLightCb.numSpotLight].diffuse = light->GetLightDiffuse();
                        spotLightCb.spotLights[spotLightCb.numSpotLight].specular = light->GetLightSpecular();
                        spotLightCb.spotLights[spotLightCb.numSpotLight].attenuation = light->GetLightAttenuation();

                        spotLightCb.numSpotLight++;
                        break;
                    }
                }

                m_pDirectionalLightCB->Update(&directionalLightCb);

                if (pointLightCb.numPointLight > 0) {
                    m_pPointLightCB->Update(&pointLightCb);
                }
                if (spotLightCb.numSpotLight > 0) {
                    m_pSpotLightCB->Update(&spotLightCb);
                }
            }
        }
    }

    // 描画
    bool CDX12Renderer::Render(CSceneBase* pScene) {

        m_pDX12Command->ResetCommandList();

        m_pDX12Command->SetRect(m_windowRect.w, m_windowRect.h);
        m_pDX12Command->SetViewPort(0, 0, m_windowRect.w, m_windowRect.h);

        CDX12Texture* pRenderTexture = nullptr;
        CDX12Texture* pDepthTexture = nullptr;

        if (m_pDepthRendring) {
            m_pDepthRendring->Rendering(pScene);
            Reset();

            pDepthTexture = dynamic_cast<CDX12DepthRendering*>(m_pDepthRendring.get())->GetDepthTexture();
        }

        if (m_pForwardRendering) {
            m_pForwardRendering->Rendering(pScene);
            Reset();

            pRenderTexture = dynamic_cast<CDX12ForwardRendering*>(m_pForwardRendering.get())->GetRenderTexture();
        }

        if (m_pTiledForwardRendering) {
            dynamic_cast<CDX12TiledForwardRendering*>(m_pTiledForwardRendering.get())->LightCulling_CS(pDepthTexture);
            m_pTiledForwardRendering->Rendering(pScene);
            Reset();

            pRenderTexture = dynamic_cast<CDX12TiledForwardRendering*>(m_pTiledForwardRendering.get())->GetRenderTexture();
        }

        if (m_pDeferredRendering) {
            m_pDeferredRendering->Rendering(pScene);
            m_pDeferredRendering->RenderingForSceneLighting(m_pRenderSprite.get());
            Reset();

            pRenderTexture = dynamic_cast<CDX12DeferredRenderig*>(m_pDeferredRendering.get())->GetRenderTexture();
        }

        if (m_pDX12PostEffect) {
            m_pDX12PostEffect->PostEffectBlur(pRenderTexture, m_pRenderSprite.get());
            //m_pDX12PostEffect->PostEffectBlur_CS(pRenderTexture);
            Reset();

            m_pDX12PostEffect->PostEffectBloom(pRenderTexture, m_pRenderSprite.get());

            pRenderTexture = m_pDX12PostEffect->GetToneMapTexture();
        }

        UINT index = m_pDX12Device->GetCurrentBackBufferIndex();

        m_pDX12Command->SetResourceBarrier(m_pRtvResource[index].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
        m_pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

        m_pDX12Command->ClearRenderTargetView(m_rtvHandle[index]);
        m_pDX12Command->ClearDepthStencilView(m_dsvHandle, D3D12_CLEAR_FLAG_DEPTH);
        m_pDX12Command->SetRenderTargetView(&m_rtvHandle[index], &m_dsvHandle);

        if (m_pRenderSprite) {
            CMeshBase* pMesh = m_pRenderSprite->GetMesh();
            if (pMesh) {
                CMaterialBase* pMaterial = pMesh->GetMaterial();
                pMaterial->SetTexture(pRenderTexture);
            }

            m_pRenderSprite->Draw();
        }

        /*UINT width = (UINT)m_windowRect.w / (RENDER_TARGET_SIZE + 1);
        UINT height = m_windowRect.h / (RENDER_TARGET_SIZE + 1);
        UINT x = 0;
        UINT y = (UINT)m_windowRect.h - height;*/

        /*if (m_pTiledForwardRendering) {
            m_pTiledForwardRendering->DrawForRenderScene(m_pRenderSprite.get(), 0, 0, m_windowRect.w, m_windowRect.h);
        }*/

        /*if (m_pDeferredRendering) {
            m_pDeferredRendering->DrawForRenderScene(m_pRenderSprite.get(), x, y, width, height);
        }*/

        /*if (m_pDepthRendring) {
            x += width * RENDER_TARGET_SIZE;
            m_pDepthRendring->DrawForRenderScene(m_pRenderSprite.get(), x, y, width, height);
        }*/

        m_pDX12Command->SetResourceBarrier(m_pRtvResource[index].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        m_pDX12Command->SetResourceBarrier(m_pDsvResource.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);

        m_pDX12Command->ExecuteComandLists();

        return m_pDX12Device->Present();
    }

    void CDX12Renderer::SetSceneConstantBuffers(UINT startSlot) {
        if (m_pCameraCB) {
            m_pCameraCB->Set(startSlot);
        }
        if (m_pDirectionalLightCB) {
            m_pDirectionalLightCB->Set(startSlot + 1);
        }
        if (m_pPointLightCB) {
            m_pPointLightCB->Set(startSlot + 2);
        }
        if (m_pSpotLightCB) {
            m_pSpotLightCB->Set(startSlot + 3);
        }
    }

    void CDX12Renderer::SetCameraCb(UINT slot, bool useCompute) {
        if (!m_pCameraCB) return;

        if (useCompute) {
            m_pCameraCB->SetCompute(slot);
            return;
        }
        m_pCameraCB->Set(slot);
    }

    void CDX12Renderer::SetDirectionalLightCb(UINT slot, bool useCompute) {
        if (!m_pDirectionalLightCB) return;

        if (useCompute) {
            m_pDirectionalLightCB->SetCompute(slot);
            return;
        }
        m_pDirectionalLightCB->Set(slot);
    }

    void CDX12Renderer::SetPointLightCb(UINT slot, bool useCompute) {
        if (!m_pPointLightCB) return;

        if (useCompute) {
            m_pPointLightCB->SetCompute(slot);
            return;
        }
        m_pPointLightCB->Set(slot);
    }

    void CDX12Renderer::SetSpotLightCb(UINT slot, bool useCompute) {
        if (!m_pSpotLightCB) return;

        if (useCompute) {
            m_pSpotLightCB->SetCompute(slot);
            return;
        }
        m_pSpotLightCB->Set(slot);
    }

    void CDX12Renderer::SetScreenParamCb(UINT slot, bool useCompute) {
        if (!m_pScreenParamCB) return;

        if (useCompute) {
            m_pScreenParamCB->SetCompute(slot);
            return;
        }
        m_pScreenParamCB->Set(slot);
    }

    void CDX12Renderer::SetCubeTexture(UINT slot) {
        if (!pCubeTexture)return;
        pCubeTexture->SetCubeTexture(slot);
    }

    // コマンドを発行しリセット
    void CDX12Renderer::Reset() {

        m_pDX12Command->ExecuteComandLists();

        m_pDX12Command->ResetCommandList();

        m_pDX12Command->SetRect(m_windowRect.w, m_windowRect.h);
        m_pDX12Command->SetViewPort(0, 0, m_windowRect.w, m_windowRect.h);
    }

    // コンスタントバッファ作成
    void CDX12Renderer::CreateConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer, UINT byteWidth) {
        pConstantBuffer = std::make_unique<CDX12ConstantBuffer>(m_pDX12Device.get(), m_pDX12Command.get());

        pConstantBuffer->CreateConstantBuffer(byteWidth);

        if (m_descriptorHeapOffsetIndex <= DESCRIPTORHEAP_MAX) {
            dynamic_cast<CDX12ConstantBuffer*>(pConstantBuffer.get())->CreateConstantBufferView(m_pCbDescriptorHeap.Get(), m_descriptorHeapOffsetIndex++);
        }
    }

    // アンオーダードアクセスバッファ作成
    void CDX12Renderer::CreateUnorderedAccessBuffer(std::unique_ptr<CUnorderedAccessBufferBase>& pUnorderedAccessBuffer, UINT byteWidth, UINT byteSize) {
        pUnorderedAccessBuffer = std::make_unique<CDX12UnorderedAccessBuffer>(m_pDX12Device.get(), m_pDX12Command.get());

        pUnorderedAccessBuffer->CreateUnorderedAccessBuffer(byteWidth, byteSize);

        if (m_descriptorHeapOffsetIndex <= DESCRIPTORHEAP_MAX) {
            dynamic_cast<CDX12UnorderedAccessBuffer*>(pUnorderedAccessBuffer.get())->CreateUnorderedAccessBufferView(m_pCbDescriptorHeap.Get(), m_descriptorHeapOffsetIndex++);
        }
    }

    // メッシュ作成
    void CDX12Renderer::CreateMesh(std::unique_ptr<CMeshBase>& pMesh, const char* meshName) {
        pMesh = std::make_unique<CDX12Mesh>(m_pDX12Device.get(), m_pDX12Command.get(), meshName);
    }

    // テクスチャ作成
    void CDX12Renderer::CreateTexture(std::unique_ptr<CTextureBase>& pTexture, const char* textureName) {
        FileInfo* pFileInfo = CFileSystem::GetFileSystem().GetFileInfoFromMap(textureName);

        if (!pFileInfo) return;

        pTexture = std::make_unique<CDX12Texture>(m_pDX12Device.get(), m_pDX12Command.get(), textureName);

        if (pTexture->LoadTexture(pFileInfo->filePath.c_str())) {
            pTexture->CreateTexture();

            dynamic_cast<CDX12Texture*>(pTexture.get())->CreateShaderResourceView(m_pCbDescriptorHeap.Get(), m_descriptorHeapOffsetIndex++);
        }
    }

    // シェーダー作成
    void CDX12Renderer::CreateShader(std::unique_ptr<CShaderPackage>& pShaderPackage, const char* shaderName) {
        FileInfo* pFileInfo = CFileSystem::GetFileSystem().GetFileInfoFromMap(shaderName);

        if (!pFileInfo) return;

        pShaderPackage = std::make_unique<CDX12ShaderPackage>(m_pDX12Device.get(), m_pDX12Command.get(), shaderName);

        for (const auto& info : SHADER_INFOS) {
            ComPtr<ID3DBlob> pBlob;

            if (pShaderPackage->CompileShader(pBlob, pFileInfo->filePath.c_str(), info.entryPoint, info.shaderModel)) {
                std::unique_ptr<CShaderBase> shader = std::make_unique<CShaderBase>(shaderName, pBlob, info.shadeType);

                pShaderPackage->AddShaderToVec(shader);
            }
        }

        if (pShaderPackage->IsCompile()) {
            CDX12ShaderPackage* pDX12Sp = dynamic_cast<CDX12ShaderPackage*>(pShaderPackage.get());

            pDX12Sp->SetDescriptorHeap(m_pCbDescriptorHeap.Get());

            D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};

            std::string name = std::string(shaderName);
            pDX12Sp->SetRTVFormats(gpsDesc, name);
            pDX12Sp->SetBlendState(gpsDesc, name);
            pDX12Sp->SetPrimitiveTopologyType(gpsDesc, name);

            pDX12Sp->InitializeGraphics(gpsDesc);

            D3D12_COMPUTE_PIPELINE_STATE_DESC cpsDesc = {};
            pDX12Sp->InitializeCompute(cpsDesc);
        }
    }

    // リソース作成
    void CDX12Renderer::CreateResource(ComPtr<ID3D12Resource>& pResource, D3D12_RESOURCE_DESC& rDesc) {
        
        CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

        m_pDX12Device->CreateResource(pResource, prop, rDesc);
    }

    // レンダーテクスチャ作成
    void CDX12Renderer::CreateRenderTexture(std::unique_ptr<CDX12Texture>& pDX12RenderTexture, D3D12_RESOURCE_DESC& rDesc, D3D12_CLEAR_VALUE& clearValue, TextureType type) {
        pDX12RenderTexture = std::make_unique<CDX12Texture>(m_pDX12Device.get(), m_pDX12Command.get());

        CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

        switch (type) {
        case TextureType::Render:
            pDX12RenderTexture->CreateTexture(rDesc, prop, clearValue);
            break;
        case TextureType::Depth:
            pDX12RenderTexture->CreateDepthTexture(rDesc, prop, clearValue);
            break;
        case TextureType::Stencil:
            pDX12RenderTexture->CreateStencilTexture(rDesc, prop, clearValue);
            break;
        }
    }

    // シェーダーリソースビュー作成
    void CDX12Renderer::CreateShaderResourceView(CDX12Texture* pDX12Texture) {
        if (pDX12Texture) {
            if (m_descriptorHeapOffsetIndex <= DESCRIPTORHEAP_MAX) {
                pDX12Texture->CreateShaderResourceView(m_pCbDescriptorHeap.Get(), m_descriptorHeapOffsetIndex++);
            }
        }
    }

    // キューブテクスチャシェーダーリソースビュー作成
    void CDX12Renderer::CreateCubeTextureShaderResourceView(CDX12Texture* pDX12Texture) {
        if (pDX12Texture) {
            if (m_descriptorHeapOffsetIndex <= DESCRIPTORHEAP_MAX) {
                pDX12Texture->CreateCubeTextureShaderResourceView(m_pCbDescriptorHeap.Get(), m_descriptorHeapOffsetIndex++);
            }
        }
    }

    // アンオーダードアクセスバッファビュー作成
    void CDX12Renderer::CreateUnorderedAccessBufferView(CDX12Texture* pDX12Texture) {
        if (pDX12Texture) {
            if (m_descriptorHeapOffsetIndex <= DESCRIPTORHEAP_MAX) {
                pDX12Texture->CreateUnorderedAccessBufferView(m_pCbDescriptorHeap.Get(), m_descriptorHeapOffsetIndex++);
            }
        }
    }

    // アンオーダードアクセスバッファ作成
    void CDX12Renderer::CreateUnorderedAccessBuffer(std::unique_ptr<CDX12UnorderedAccessBuffer>& pUnorderedAccessBuffer) {
        pUnorderedAccessBuffer = std::make_unique<CDX12UnorderedAccessBuffer>(m_pDX12Device.get(), m_pDX12Command.get());
    }

    // アンオーダードアクセスバッファビュー作成
    void CDX12Renderer::CreateUnorderedAccessBufferView(CDX12UnorderedAccessBuffer* pUnorderedAccessBuffer, ComPtr<ID3D12Resource>& pResource, D3D12_UNORDERED_ACCESS_VIEW_DESC& uavDesc) {
        if (pUnorderedAccessBuffer && pResource) {
            pUnorderedAccessBuffer->SetResource(pResource);

            if (m_descriptorHeapOffsetIndex <= DESCRIPTORHEAP_MAX) {
                pUnorderedAccessBuffer->CreateUnorderedAccessBufferView(nullptr, uavDesc, m_pCbDescriptorHeap.Get(), m_descriptorHeapOffsetIndex++);
                pUnorderedAccessBuffer->CreateShaderResourceView(m_pCbDescriptorHeap.Get(), m_descriptorHeapOffsetIndex++);
            }
        }
    }

    // アンオーダードアクセスバッファビュー作成
    void CDX12Renderer::CreateUnorderedAccessBufferView(CDX12UnorderedAccessBuffer* pUnorderedAccessBuffer, ID3D12Resource* pResource, D3D12_UNORDERED_ACCESS_VIEW_DESC& uavDesc) {
        if (pUnorderedAccessBuffer && pResource) {
            if (m_descriptorHeapOffsetIndex <= DESCRIPTORHEAP_MAX) {
                pUnorderedAccessBuffer->CreateUnorderedAccessBufferView(pResource, uavDesc, m_pCbDescriptorHeap.Get(), m_descriptorHeapOffsetIndex++);
                pUnorderedAccessBuffer->CreateShaderResourceView(m_pCbDescriptorHeap.Get(), m_descriptorHeapOffsetIndex++);
            }
        }
    }

} // namespace
