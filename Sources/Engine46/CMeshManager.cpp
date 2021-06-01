/**
 * @file CMeshManager.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/23
 */

#include "CMeshManager.h"
#include "CMesh.h"
#include "CRenderer.h"
#include "CActor.h"

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

		if (!pMesh) {
			std::unique_ptr<CMeshBase> mesh = std::make_unique<CMeshBase>();

			pMesh = mesh.get();

			this->AddMeshToMap(meshName, mesh);
		}

		return pMesh;
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

	// アクターへメッシュを設定
	void CMeshManager::SetMeshToActor(CActorBase* pActor) {
		std::unique_ptr<CMeshBase> pMesh;

		pRenderer->CreateMesh(pMesh);

		if (pMesh) {
			pActor->SetMesh(pMesh.get());
			this->AddMeshToMap(pMesh->GetMeshName(), pMesh);
		}
	}

} // namespace
