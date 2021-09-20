/**
 * @file CLight.cpp
 * @brief
 * @author 木村優
 * @date 2021/07/04
 */

#include "CLight.h"
#include "CRenderer.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CConstantBuffer.h"
#include "CSprite.h"

namespace Engine46 {

	// コンストラクタ
	CLight::CLight(const char* lightName) :
		CActorBase((int)ClassType::Light, lightName, Transform()),
		m_lightID(0),
		m_lightDiffuse(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_lightSpecular(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_lightAmbinet(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_lightEmissive(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_lightAttenuation(VECTOR4(0.1f, 0.1f, 0.0f, 1.0f))
	{}

	// デストラクタ
	CLight::~CLight()
	{}

	// リソースの初期化
	void CLight::InitializeResource(CRendererBase* pRenderer) {
		
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
	void CLight::Update() {

	}

} // namespace
