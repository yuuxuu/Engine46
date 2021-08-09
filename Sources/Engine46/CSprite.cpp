/**
 * @file CSprite.cpp
 * @brief
 * @author 木村優
 * @date 2001/05/06
 */

#include "CSprite.h"
#include "CRenderer.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CConstantBuffer.h"

namespace Engine46 {

	// コンストラクタ
	CSprite::CSprite(const char* actorName) :
		CActorBase((int)ClassType::Sprite, actorName, Transform(VECTOR3(), VECTOR3(), VECTOR3(10.0f, 10.0f, 10.0f)))
	{}

	// デストラクタ
	CSprite::~CSprite()
	{}

	// リソースの初期化
	void CSprite::InitializeResource(CRendererBase* pRenderer) {

		if (pRenderer) {
			std::unique_ptr<CConstantBufferBase> pConstantBuffer;
			pRenderer->CreateConstantBuffer(pConstantBuffer);
			SetWorldConstantBuffer(pConstantBuffer);

			if (m_pMaterial && !m_pMaterial->IsInitialize()) {
				std::unique_ptr<CConstantBufferBase> pMaterialConstantBuffer;
				pRenderer->CreateConstantBuffer(pMaterialConstantBuffer);

				m_pMaterial->SetMaterialConstantBuffer(pMaterialConstantBuffer);
			}

		}

		if (m_pMesh && !m_pMesh->IsInitialize()) {
			m_pMesh->ReserveVertex(4);

			vertexInfo info;

			info.vertex = VECTOR3(-1.0f, 1.0f, 0.0f);
			info.color = VECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
			info.uv = VECTOR2(0.0f, 0.0f);
			info.normal = VECTOR3(0.0f, 0.0f, -1.0f);
			m_pMesh->AddVertexInfo(info);

			info.vertex = VECTOR3(1.0f, 1.0f, 0.0f);
			info.color = VECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
			info.uv = VECTOR2(1.0f, 0.0f);
			info.normal = VECTOR3(0.0f, 0.0f, -1.0f);
			m_pMesh->AddVertexInfo(info);

			info.vertex = VECTOR3(-1.0f, -1.0f, 0.0f);
			info.color = VECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
			info.uv = VECTOR2(0.0f, 1.0f);
			info.normal = VECTOR3(0.0f, 0.0f, -1.0f);
			m_pMesh->AddVertexInfo(info);

			info.vertex = VECTOR3(1.0f, -1.0f, 0.0f);
			info.color = VECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
			info.uv = VECTOR2(1.0f, 1.0f);
			info.normal = VECTOR3(0.0f, 0.0f, -1.0f);
			m_pMesh->AddVertexInfo(info);

			m_pMesh->ReserveIndex(6);

			m_pMesh->AddIndex(0);
			m_pMesh->AddIndex(1);
			m_pMesh->AddIndex(3);
			m_pMesh->AddIndex(0);
			m_pMesh->AddIndex(3);
			m_pMesh->AddIndex(2);

			m_pMesh->Create();
		}
	}

	// 更新
	void CSprite::Update() {
		
	}

} // namespace