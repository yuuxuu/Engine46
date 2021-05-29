/**
 * @file CMeshManager.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/23
 */

#include "CMeshManager.h"
#include "CMesh.h"
#include "CRenderer.h"

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
		auto itr = m_pMapMesh.find(meshName);

		if (itr == m_pMapMesh.end()) {
			std::unique_ptr<CMeshBase> mesh = std::make_unique<CMeshBase>();

			CMeshBase* pMesh = mesh.get();

			this->AddMeshToMap(meshName, mesh);

			return pMesh;
		}

		return itr->second.get();
	}

	// メッシュをマップへ追加
	void CMeshManager::AddMeshToMap(const char* name, std::unique_ptr<CMeshBase>& pMesh) {
		auto itr = m_pMapMesh.find(name);

		if (itr == m_pMapMesh.end()) {
			m_pMapMesh[name] = std::move(pMesh);
		}
	}

	// メッシュを取得
	CMeshBase* CMeshManager::GetMesh(const char* name) {
		auto itr = m_pMapMesh.find(name);

		if (itr != m_pMapMesh.end()) {
			return itr->second.get();
		}

		return nullptr;
	}

} // namespace
