/**
 * @file CMaterialManager.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/23
 */

#include "CMaterialManager.h"
#include "CMaterial.h"
#include "CActor.h"

#include "GraphicsAPI/CRenderer.h"

namespace Engine46 {

    // コンストラクタ
    CMaterialManager::CMaterialManager(CRendererBase* pRenderer) :
        pRenderer(pRenderer),
        m_materialCount(0)
    {}

    // デストラクタ
    CMaterialManager::~CMaterialManager()
    {}

    // マテリアルを作成
    CMaterialBase* CMaterialManager::CreateMaterial(const char* materialName) {
        CMaterialBase* pMaterial = GetMaterialFromMap(materialName);

        if (pMaterial) return pMaterial;

        std::unique_ptr<CMaterialBase> material = std::make_unique<CMaterialBase>(materialName);

        if (material) {
            std::unique_ptr<CConstantBufferBase> pMaterialConstantBuffer;
            pRenderer->CreateConstantBuffer(pMaterialConstantBuffer, sizeof(materialCB));
            material->SetMaterialConstantBuffer(pMaterialConstantBuffer);

            pMaterial = material.get();

            this->AddMaterialToMap(materialName, material);

            return pMaterial;
        }

        return nullptr;
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

} // namespace