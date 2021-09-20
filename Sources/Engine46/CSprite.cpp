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
			pRenderer->CreateConstantBuffer(pConstantBuffer, sizeof(worldCB));
			SetWorldConstantBuffer(pConstantBuffer);

			if (pMaterial && !pMaterial->IsInitialize()) {
				std::unique_ptr<CConstantBufferBase> pMaterialConstantBuffer;
				pRenderer->CreateConstantBuffer(pMaterialConstantBuffer, sizeof(materialCB));
				pMaterial->SetMaterialConstantBuffer(pMaterialConstantBuffer);
			}
		}

		if (pMesh && !pMesh->IsInitialize()) {
			pMesh->ReserveVertex(4);

			vertexInfo info;

			info.vertex = VECTOR3(-1.0f, 1.0f, 0.0f);
			info.color = VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			info.uv = VECTOR2(0.0f, 0.0f);
			info.normal = VECTOR3(0.0f, 0.0f, -1.0f);
			pMesh->AddVertexInfo(info);

			info.vertex = VECTOR3(1.0f, 1.0f, 0.0f);
			info.color = VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			info.uv = VECTOR2(1.0f, 0.0f);
			info.normal = VECTOR3(0.0f, 0.0f, -1.0f);
			pMesh->AddVertexInfo(info);

			info.vertex = VECTOR3(-1.0f, -1.0f, 0.0f);
			info.color = VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			info.uv = VECTOR2(0.0f, 1.0f);
			info.normal = VECTOR3(0.0f, 0.0f, -1.0f);
			pMesh->AddVertexInfo(info);

			info.vertex = VECTOR3(1.0f, -1.0f, 0.0f);
			info.color = VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			info.uv = VECTOR2(1.0f, 1.0f);
			info.normal = VECTOR3(0.0f, 0.0f, -1.0f);
			pMesh->AddVertexInfo(info);

			pMesh->CreateVertexBuffer(PRIMITIVE_TOPOLOGY_TYPE::TRIANGLESTRIP);

			pMesh->ReserveIndex(6);

			pMesh->AddIndex(0);
			pMesh->AddIndex(1);
			pMesh->AddIndex(3);
			pMesh->AddIndex(0);
			pMesh->AddIndex(3);
			pMesh->AddIndex(2);

			pMesh->CreateIndexBuffer();
		}
	}

	// 更新
	void CSprite::Update() {
		
	}

} // namespace