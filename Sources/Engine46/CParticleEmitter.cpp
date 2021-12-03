﻿/**
 * @file CParticleEmitter.h
 * @brief
 * @author 木村優
 * @date 2021/11/24
 */

#include "CParticleEmitter.h"
#include "CGameSystem.h"
#include "CShaderManager.h"
#include "CRendererSystem.h"
#include "CTextureManager.h"

#include "GraphicsAPI/DirectX12/CDX12Renderer.h"

namespace Engine46 {

    // コンストラクタ
    CParticleEmitter::CParticleEmitter(const char* particleEmitterName) :
        CActorBase((int)ActorType::ParticleEmitter, particleEmitterName, Transform()),
        m_maxParticle(DEFAULT_MAX_PARTICLE)
    {}

    // デストラクタ
    CParticleEmitter::~CParticleEmitter()
    {}

    // 初期化
    void CParticleEmitter::Initialize() {

        CDX12Renderer* pDX12Renderer = dynamic_cast<CDX12Renderer*>(CRendererSystem::GetRendererSystem().GetRenderer());
        if (!pDX12Renderer) return;

        pDX12Renderer->CreateUnorderedAccessBuffer(m_pParticleUab, sizeof(Particle), m_maxParticle);
        if (!m_pParticleUab) return;

        m_pParticleUab->CreateVertexBufferView();

        CTextureManager* textureManager = CGameSystem::GetGameSystem().GetTextureManager();
        pParticleTexture = textureManager->CreateTexture("particle.png");
    }

    // 更新
    void CParticleEmitter::Update() {
        /*if (m_pParticleUab) {
            CShaderManager* pShaderManager = CGameSystem::GetGameSystem().GetShaderManager();

            CShaderPackage* pSp = pShaderManager->CreateShaderPackage("CS_GPUParticle.hlsl");
            if (pSp) {
                pSp->SetShader();

                m_pParticleUab->SetCompute((UINT)MyRS_CS_GpuParticle::UAV_0);

                m_pParticleUab->Dispatch(m_maxParticle / 100, 1, 1);
            }
        }*/
    }

    // 描画
    void CParticleEmitter::Draw() {
        if (m_pParticleUab) {
            CShaderManager* pShaderManager = CGameSystem::GetGameSystem().GetShaderManager();

            CShaderPackage* pSp = pShaderManager->CreateShaderPackage("CS_GPUParticle.hlsl");
            if (pSp) {
                pSp->SetShader();

                m_pParticleUab->SetCompute((UINT)MyRS_CS_GpuParticle::UAV_0);

                m_pParticleUab->Dispatch(m_maxParticle / 1024, 1, 1);
            }

            pSp = pShaderManager->CreateShaderPackage("PointSprite.hlsl");
            if (pSp) {
                pSp->SetShader();
                pSp->SetSceneConstantBufferToShader((UINT)MyRS_Model::CBV_CAMERA);

                Matrix matW = GetBillboradMatrix();
                matW.dx_m = DirectX::XMMatrixTranspose(matW.dx_m);

                worldCB cb = {
                    matW,
                };
                m_pWorldConstantBuffer->Update(&cb);
                m_pWorldConstantBuffer->Set((UINT)CB_TYPE::WORLD);

                if (pParticleTexture) {
                    pParticleTexture->Set((UINT)MyRS_Model::SRV_diffuse);
                }

                m_pParticleUab->Draw();
            }
        }
    }

    // 更新
    void CParticleEmitter::Update(std::vector<Particle>& vecParticle) {
        if (m_pParticleUab) {
            m_pParticleUab->WriteBufferData(&vecParticle[0], sizeof(Particle) * static_cast<int>(vecParticle.size()));
        }
    }

} // namespace
