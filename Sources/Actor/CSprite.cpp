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
		CActorBase(1, "Sprite", Transform())
	{}

	// デストラクタ
	CSprite::~CSprite()
	{}

	// 初期化
	void CSprite::Initialize() {

		m_pMesh->ReserveVertex(4);

		m_pMesh->AddVertex(VECTOR3(-1.0f, 1.0f, 0.0f));
		m_pMesh->AddVertex(VECTOR3( 1.0f, 1.0f, 0.0f));
		m_pMesh->AddVertex(VECTOR3(-1.0f,-1.0f, 0.0f));
		m_pMesh->AddVertex(VECTOR3( 1.0f,-1.0f, 0.0f));

		m_pMesh->AddColor(VECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
		m_pMesh->AddColor(VECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
		m_pMesh->AddColor(VECTOR4(0.0f, 0.0f, 1.0f, 1.0f));
		m_pMesh->AddColor(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

		m_pMesh->AddUV(VECTOR2(0.0f, 0.0f));
		m_pMesh->AddUV(VECTOR2(1.0f, 0.0f));
		m_pMesh->AddUV(VECTOR2(0.0f, 1.0f));
		m_pMesh->AddUV(VECTOR2(1.0f, 1.0f));

		m_pMesh->ReserveIndex(6);

		m_pMesh->AddIndex(0);
		m_pMesh->AddIndex(1);
		m_pMesh->AddIndex(3);
		m_pMesh->AddIndex(0);
		m_pMesh->AddIndex(3);
		m_pMesh->AddIndex(2);

		m_pMesh->Create();
	}

} // namespace