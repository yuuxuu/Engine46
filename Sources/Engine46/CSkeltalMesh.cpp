/**
 * @file CSkeltalMesh.h
 * @brief
 * @author 木村優
 * @date 2021/05/06
 */

#include "CSkeltalMesh.h"
#include "CMesh.h"

namespace Engine46 {

    // コンストラクタ
    CSkeltalMesh::CSkeltalMesh()
    {}

    // デストラクタ
    CSkeltalMesh::~CSkeltalMesh()
    {}

    // モデル描画
    void CSkeltalMesh::Draw() {
        for (const auto& pMesh : m_pVecMesh) {
            pMesh->Set();
            pMesh->Draw();
        }
    }

    // メッシュとマテリアルのペアを追加
    void CSkeltalMesh::AddMesh(CMeshBase* pMesh) {
        if (!pMesh) return;

        m_pVecMesh.emplace_back(pMesh);
    }

} // namespace