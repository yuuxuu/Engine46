/**
 * @file CActorManager.cpp
 * @brief
 * @author 木村優
 * @date 2019/01/02
 */

#include "CActorManager.h"
#include "CActor.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CConstantBuffer.h"
#include "CShaderPackage.h"
#include "CRenderer.h"
#include "CInput.h"
#include "CSprite.h"
#include "CCamera.h"
#include "math.h"

namespace Engine46 {

	constexpr const char* g_ActorListFileName = "ActorListData.bin";

	// コンストラクタ
	CActorManager::CActorManager(CRendererBase* pRenderer) :
		pRenderer(pRenderer)
	{}

	// デストラクタ
	CActorManager::~CActorManager()
	{}

	// 初期化
	bool CActorManager::Initialize() {

		pRootActor = this->CreateActor((int)ClassType::Root);

		CActorBase* camera = this->CreateActor((int)ClassType::Camera);
		camera->Initialize();

		CActorBase* sprite = this->CreateActor((int)ClassType::Sprite);

		return true;
	}

	// オブジェクト作成
	CActorBase* CActorManager::CreateActor(int classID) {
		std::unique_ptr<CActorBase> actor;
		RECT rect;

		switch ((ClassType)classID) {
		case ClassType::Root:
			actor = std::make_unique<CActorBase>();
			break;
		case ClassType::Camera:
			rect = pRenderer->GetWindowRect();
			actor = std::make_unique<CCamera>(rect.w, rect.h);
			break;
		case ClassType::Sprite:
			actor = std::make_unique<CSprite>();
			break;
		}

		actor->CActorBase::Initialize();

		CActorBase* pActor = actor.get();

		if (classID != (int)ClassType::Root) {
			pRootActor->AddChiledActorList(pActor);
		}

		this->AddActorToVec(actor);

		return pActor;
	}

	// オブジェクトのメッシュを設定
	void CActorManager::SetMesh(CActorBase* pActor, CMeshBase* pMesh) {
		if (pActor) {
			pActor->SetMesh(pMesh);
		}
	}

	// オブジェクトのマテリアルを設定
	void CActorManager::SetMaterial(CActorBase* pActor, CMaterialBase* pMaterial) {
		if (pActor) {
			pActor->SetMaterial(pMaterial);
		}
	}

	// オブジェクトにシェーダーパッケージを設定
	void CActorManager::SetShaderPackage(CActorBase* pActor, CShaderPackage* pShaderPackage) {
		if (pActor) {
			pActor->SetShaderPackage(pShaderPackage);
		}
	}

	// オブジェクトにインプットを設定
	void CActorManager::SetInput(CActorBase* pActor, CInput* pInput) {
		if (pActor) {
			pActor->SetInput(pInput);
		}
	}

	// オブジェクトを保存
	bool CActorManager::SaveActor() {

		std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary;

		std::ofstream ofs;
		ofs.open(g_ActorListFileName, mode);

		if (!ofs.is_open()) return false;

		for (const auto& actor : m_pVecActor) {
			actor->Save(ofs);
		}

		return true;
	}

	// オブジェクトを読み込み
	bool CActorManager::LoadActor() {

		std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary;

		std::ifstream ifs;
		ifs.open(g_ActorListFileName, mode);

		if (!ifs.is_open()) return false;

		while (true) {

			int id = -1;
			ifs.read((char*)&id, sizeof(int));

			if (ifs.eof()) break;

			CActorBase* pActor = this->CreateActor(id);

			pActor->Load(ifs);
		}

		std::cout << g_ActorListFileName << "を読み込みしました。" << std::endl;

		this->ConnectActor();

		return true;
	}

	// オブジェクト同士の接続
	void CActorManager::ConnectActor() {
		for (const auto& actor : m_pVecActor) {
			int id = actor->GetParentActorID();
			if (id > -1) {
				actor->ConnectParentActor(m_pVecActor[id].get());
			}

			for (auto id : actor->GetChiledActorIDList()) {
				actor->AddChiledActorList(m_pVecActor[id].get());
			}
		}
	}

} // namespace