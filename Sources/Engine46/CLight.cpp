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
			m_pSprite = std::make_unique<CSprite>("LightSprite");
			m_pSprite->SetMesh("LightMesh");
			m_pSprite->SetMaterial("LightMaterial");
			m_pSprite->SetTexture("particle.png");
			m_pSprite->SetShaderPackage("CPUParticle.hlsl");
			m_pSprite->InitializeResource(pRenderer);
		}
	}

	// 更新
	void CLight::Update() {
		if (m_pSprite) {
			m_pSprite->SetTransform(m_transform);
		}
	}

	// 更新
	void CLight::Draw() {
		if (m_pSprite) {
			m_pSprite->Draw();
		}
	}

} // namespace
