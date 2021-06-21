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
#include "CRenderer.h"

namespace Engine46 {

	// コンストラクタ
	CSprite::CSprite() :
		CActorBase((int)ClassType::Sprite, "Sprite", Transform(VECTOR3(), VECTOR3(), VECTOR3(10.0f, 10.0f, 10.0f)))
	{}

	// デストラクタ
	CSprite::~CSprite()
	{}

	// 初期化
	void CSprite::InitializeResource(CRendererBase* pRenderer) {

		if (pRenderer) {
			std::unique_ptr<CConstantBufferBase> pConstantBuffer;
			pRenderer->CreateConstantBuffer(pConstantBuffer);
			SetConstantBuffer(pConstantBuffer);

			std::unique_ptr<CMeshBase> pMesh;
			pRenderer->CreateMesh(pMesh);

			pMesh->ReserveVertex(4);

			vertexInfo info;

			info.vertex = VECTOR3(-1.0f, 1.0f, 0.0f);
			info.color = VECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
			info.uv = VECTOR2(0.0f, 0.0f);
			pMesh->AddVertexInfo(info);

			info.vertex = VECTOR3(1.0f, 1.0f, 0.0f);
			info.color = VECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
			info.uv = VECTOR2(1.0f, 0.0f);
			pMesh->AddVertexInfo(info);

			info.vertex = VECTOR3(-1.0f, -1.0f, 0.0f);
			info.color = VECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
			info.uv = VECTOR2(0.0f, 1.0f);
			pMesh->AddVertexInfo(info);

			info.vertex = VECTOR3(1.0f, -1.0f, 0.0f);
			info.color = VECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
			info.uv = VECTOR2(1.0f, 1.0f);
			pMesh->AddVertexInfo(info);

			pMesh->ReserveIndex(6);

			pMesh->AddIndex(0);
			pMesh->AddIndex(1);
			pMesh->AddIndex(3);
			pMesh->AddIndex(0);
			pMesh->AddIndex(3);
			pMesh->AddIndex(2);

			SetMesh(pMesh);

			std::unique_ptr<CMaterialBase> pMaterial;
			pRenderer->CreateMaterial(pMaterial);
			SetMaterial(pMaterial);
		}
	}

	// 更新
	void CSprite::Update() {
		
	}

} // namespace