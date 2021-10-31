/**
 * @file CLight.cpp
 * @brief
 * @author 木村優
 * @date 2021/07/04
 */

#include "CLight.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CConstantBuffer.h"
#include "CSprite.h"

#include "GraphicsAPI/CRenderer.h"

namespace Engine46 {

    // コンストラクタ
    CLight::CLight(const char* lightName) :
        CActorBase((int)ActorType::Light, lightName, Transform()),
        m_lightID(0),
        m_lightDiffuse(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_lightSpecular(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_lightAmbinet(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_lightEmissive(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_lightAttenuation(VECTOR4(0.0f, 0.0f, 0.1f, 1.0f))
    {}

    // デストラクタ
    CLight::~CLight()
    {}

    // リソースの初期化
    void CLight::InitializeResource(CRendererBase* pRenderer) {

        CActorBase::InitializeResource(pRenderer);

        if (pMesh) {
            pMesh->CreateSpriteMesh();
        }
    }

    // 更新
    void CLight::Update() {
        
    }

    // 描画
    void CLight::Draw() {
        Matrix matW = GetBillboradMatrix();
        matW.dx_m = DirectX::XMMatrixTranspose(matW.dx_m);

        worldCB cb = {
            matW,
        };
        m_pWorldConstantBuffer->Update(&cb);

        CActorBase::Draw();
    }

} // namespace
