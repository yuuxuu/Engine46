/**
 * @file CMaterialManager.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/23
 */

#include "CMaterialManager.h"
#include "CMaterial.h"
#include "CRenderer.h"

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
		auto itr = m_pMapMaterial.find(materialName);

		if (itr == m_pMapMaterial.end()) {
			std::unique_ptr<CMaterialBase> material = std::make_unique<CMaterialBase>();

			CMaterialBase* pMaterial = material.get();

			this->AddMaterialToMap(materialName, material);

			return pMaterial;
		}

		return itr->second.get();
	}

	// マテリアルをマップへ追加
	void CMaterialManager::AddMaterialToMap(const char* name, std::unique_ptr<CMaterialBase>& pMaterial) {
		auto itr = m_pMapMaterial.find(name);

		if (itr == m_pMapMaterial.end()) {
			m_pMapMaterial[name] = std::move(pMaterial);
		}
	}

	// マテリアルを取得
	CMaterialBase* CMaterialManager::GetMaterial(const char* name) {
		auto itr = m_pMapMaterial.find(name);

		if (itr != m_pMapMaterial.end()) {
			return itr->second.get();
		}

		return nullptr;
	}

} // namespace