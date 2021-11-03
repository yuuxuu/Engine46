/**
 * @file CMeshManager.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/23
 */

#include "CMeshManager.h"
#include "CMesh.h"
#include "CActor.h"

#include "GraphicsAPI/CRenderer.h"

namespace Engine46 {

    // コンストラクタ
    CMeshManager::CMeshManager(CRendererBase* pRenderer) :
        pRenderer(pRenderer)
    {}

    // デストラクタ
    CMeshManager::~CMeshManager()
    {}

    // メッシュ作成
    CMeshBase* CMeshManager::CreateMesh(const char* meshName) {
        CMeshBase* pMesh = GetMeshFromMap(meshName);

        if (pMesh) return pMesh;

        std::unique_ptr<CMeshBase> mesh;

        pRenderer->CreateMesh(mesh, meshName);

        if (mesh) {
            pMesh = mesh.get();

            this->AddMeshToMap(meshName, mesh);

            return pMesh;
        }

        return nullptr;
    }

    // メッシュをマップへ追加
    void CMeshManager::AddMeshToMap(const char* name, std::unique_ptr<CMeshBase>& pMesh) {

        if (!GetMeshFromMap(name)) {
            m_pMapMesh[name] = std::move(pMesh);
        }
    }

    // メッシュを取得
    CMeshBase* CMeshManager::GetMeshFromMap(const char* name) {
        auto itr = m_pMapMesh.find(name);

        if (itr != m_pMapMesh.end()) {
            return itr->second.get();
        }

        return nullptr;
    }

} // namespace
