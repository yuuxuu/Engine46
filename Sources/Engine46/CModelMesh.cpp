/**
 * @file CModelMesh.h
 * @brief
 * @author 木村優
 * @date 2021/05/06
 */

#include "CModelMesh.h"
#include "CMesh.h"

namespace Engine46 {

    // コンストラクタ
    CModelMesh::CModelMesh(const std::string& modelName) :
        m_modelName(modelName)
    {
        m_modelName.resize(m_modelName.size());
    }

    // デストラクタ
    CModelMesh::~CModelMesh()
    {}

    // モデル描画
    void CModelMesh::Draw() {
        for (const auto& pMesh : m_pVecMesh) {
            pMesh->Set();
            pMesh->Draw();
        }
    }

    // メッシュとマテリアルのペアを追加
    void CModelMesh::AddMesh(CMeshBase* pMesh) {
        if (!pMesh) return;

        m_pVecMesh.emplace_back(pMesh);
    }

} // namespace