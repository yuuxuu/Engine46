/**
 * @file CMaterialManager.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/23
 */

#include "CMaterialManager.h"
#include "CMaterial.h"
#include "CRenderer.h"
#include "CActor.h"

namespace Engine46 {

	// コンストラクタ
	CMaterialManager::CMaterialManager(CRendererBase* pRenderer) :
		pRenderer(pRenderer)
	{}

	// デストラクタ
	CMaterialManager::~CMaterialManager()
	{}

	// マテリアルを作成
	CMaterialBase* CMaterialManager::CreateMaterial(const char* materialName) {
		CMaterialBase* pMaterial = GetMaterialFromMap(materialName);

		if (!pMaterial) {
			std::unique_ptr<CMaterialBase> material = std::make_unique<CMaterialBase>();

			pMaterial = material.get();

			this->AddMaterialToMap(materialName, material);
		}

		return pMaterial;
	}

	// マテリアルをマップへ追加
	void CMaterialManager::AddMaterialToMap(const char* name, std::unique_ptr<CMaterialBase>& pMaterial) {

		if (!GetMaterialFromMap(name)) {
			m_pMapMaterial[name] = std::move(pMaterial);
		}
	}

	// マテリアルを取得
	CMaterialBase* CMaterialManager::GetMaterialFromMap(const char* name) {
		auto itr = m_pMapMaterial.find(name);

		if (itr != m_pMapMaterial.end()) {
			return itr->second.get();
		}

		return nullptr;
	}

	// アクターへマテリアルを設定
	void CMaterialManager::SetMaterialToActor(CActorBase* pActor) {
		std::unique_ptr<CMaterialBase> pMaterial;

		pRenderer->CreateMaterial(pMaterial);

		if (pMaterial) {
			pActor->SetMaterial(pMaterial.get());
			this->AddMaterialToMap(pMaterial->GetMaterialName(), pMaterial);
		}
	}

} // namespace