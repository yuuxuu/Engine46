/**
 * @file CSprite.cpp
 * @brief
 * @author 木村優
 * @date 2001/05/06
 */

#include "CSprite.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CConstantBuffer.h"

#include "GraphicsAPI/CRenderer.h"

namespace Engine46 {

    // コンストラクタ
    CSprite::CSprite(const char* actorName) :
        CActorBase((int)ActorType::Sprite, actorName, Transform(VECTOR3(), VECTOR3(), VECTOR3(10.0f, 10.0f, 10.0f)))
    {}

    // デストラクタ
    CSprite::~CSprite()
    {}

    // リソースの初期化
    void CSprite::InitializeResource(CRendererBase* pRenderer) {

        CActorBase::InitializeResource(pRenderer);

        if (pMesh) {
            pMesh->CreateSpriteMesh();
        }
    }

    // 更新
    void CSprite::Update() {

    }

    // 描画
    void CSprite::Draw() {
        Matrix matW = GetWorldMatrix();
        matW.dx_m = DirectX::XMMatrixTranspose(matW.dx_m);

        worldCB cb = {
            matW,
        };
        m_pWorldConstantBuffer->Update(&cb);

        CActorBase::Draw();
    }

} // namespace