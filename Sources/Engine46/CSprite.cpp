/**
 * @file CSprite.cpp
 * @brief
 * @author 木村優
 * @date 2001/05/06
 */

#include "CSprite.h"

namespace Engine46 {

	// コンストラクタ
	CSprite::CSprite() :
		CActorBase((int)ClassType::Sprite, "Sprite", Transform(VECTOR3(), VECTOR3(), VECTOR3(10.0f, 10.0f, 10.0f)))
	{}

	// デストラクタ
	CSprite::~CSprite()
	{}

	// 初期化
	void CSprite::Initialize() {

		if (m_pMesh) {
			m_pMesh->ReserveVertex(4);

			vertexInfo info;

			info.vertex = VECTOR3(-1.0f, 1.0f, 0.0f);
			info.color	= VECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
			info.uv		= VECTOR2(0.0f, 0.0f);
			m_pMesh->AddVertexInfo(info);

			info.vertex = VECTOR3(1.0f, 1.0f, 0.0f);
			info.color	= VECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
			info.uv		= VECTOR2(1.0f, 0.0f);
			m_pMesh->AddVertexInfo(info);

			info.vertex = VECTOR3(-1.0f, -1.0f, 0.0f);
			info.color	= VECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
			info.uv		= VECTOR2(0.0f, 1.0f);
			m_pMesh->AddVertexInfo(info);

			info.vertex = VECTOR3(1.0f, -1.0f, 0.0f);
			info.color	= VECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
			info.uv		= VECTOR2(1.0f, 1.0f);
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

		if (m_pMaterial) {
			m_pMaterial->CreateConstantBuffer();
		}
	}

	// 更新
	void CSprite::Update() {
		
	}

} // namespace