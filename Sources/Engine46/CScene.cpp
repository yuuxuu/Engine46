/**
 * @file CScene.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CScene.h"
#include "CDataRecord.h"
#include "CActor.h"
#include "CCamera.h"
#include "CLight.h"

namespace Engine46 {

	// コンストラクタ
	CSceneBase::CSceneBase() :
		pParentScene(nullptr),
		m_parentSceneID(-1),
		m_SceneID(0),
		m_SceneName("Scene_" + std::to_string(m_SceneID))
	{
		m_SceneName.resize(m_SceneName.size());
	}

	// コンストラクタ
	CSceneBase::CSceneBase(const char* sceneName) :
		pParentScene(nullptr),
		m_parentSceneID(-1),
		m_SceneID(0),
		m_SceneName(sceneName)
	{
		m_SceneName.resize(m_SceneName.size());
	}

	// デストラクタ
	CSceneBase::~CSceneBase() 
	{}

	//初期化
	void CSceneBase::Initialize() {

		vecDataRecords.clear();

		vecDataRecords.emplace_back(CDataRecordBase(offsetof(CSceneBase, m_SceneID), sizeof(m_SceneID)));
		vecDataRecords.emplace_back(CStrDataRecord(offsetof(CSceneBase, m_SceneName), m_SceneName));
		vecDataRecords.emplace_back(CPtrDataRecord(m_parentSceneID));
		vecDataRecords.emplace_back(CListDataRecord(m_chiledSceneIDList));
	}

	// シーン更新
	void CSceneBase::Update() {
		if (pRootActor) {
			pRootActor->Update();
		}
	}

	// シーン描画
	void CSceneBase::Draw() {
		if (pRootActor) {
			pRootActor->Draw();
		}
	}

	// シーン出力
	bool CSceneBase::Save(std::ofstream& ofs) {

		for (auto& records : vecDataRecords) {
			records.WriteData(ofs, (char*)this);
		}

		return true;
	}

	// シーン読み込み
	bool CSceneBase::Load(std::ifstream& ifs) {

		for (auto& records : vecDataRecords) {
			if (&records == &vecDataRecords[0]) continue;

			records.ReadData(ifs, (char*)this);
		}

		return true;
	}

	// 親シーンを接続
	void CSceneBase::ConnectParentScene(CSceneBase* pParentScene) {
		
		if (this->pParentScene == pParentScene) return;
		
		this->pParentScene = pParentScene;

		if (pParentScene) {
			m_parentSceneID = pParentScene->m_SceneID;
		}
		else {
			m_parentSceneID = -1;
		}
	}

	// 子シーンを追加
	void CSceneBase::AddChiledSceneList(CSceneBase* pChiledScene) {
		if (pChiledScene) {
			auto it = std::find(m_chiledSceneIDList.begin(), m_chiledSceneIDList.end(), pChiledScene->m_SceneID);

			if (it == m_chiledSceneIDList.end()) {
				pChiledScene->ConnectParentScene(this);

				pChiledSceneList.emplace_back(pChiledScene);

				m_chiledSceneIDList.emplace_back(pChiledScene->m_SceneID);
			}
		}
	}

	// シーン内のアクターを名前で取得
	CActorBase* CSceneBase::GetActorFromActorName(std::string& actorName) {
		if (pRootActor) {
			if (pRootActor->GetActorName() == actorName) {
				return pRootActor;
			}
			return this->GetActorRecursiveInName(pRootActor, actorName);
		}

		return nullptr;
	}

	// シーン内のカメラを取得
	CCamera* CSceneBase::GetCameraFromScene() {
		if (pRootActor) {
			if (pRootActor->GetClassID() == (int)ClassType::Camera) {
				return dynamic_cast<CCamera*>(pRootActor);
			}
			CActorBase* pCamera = this->GetActorRecursiveInClass(pRootActor, (int)ClassType::Camera);
			return dynamic_cast<CCamera*>(pCamera);
		}

		return nullptr;
	}

	// シーン内のライトを取得
	CLight* CSceneBase::GetLightFromScene() {
		if (pRootActor) {
			if (pRootActor->GetClassID() == (int)ClassType::Light) {
				return dynamic_cast<CLight*>(pRootActor);
			}
			CActorBase* pLight = this->GetActorRecursiveInClass(pRootActor, (int)ClassType::Light);
			return dynamic_cast<CLight*>(pLight);
		}

		return nullptr;
	}

	// シーン内カメラを全て取得
	std::vector<CCamera*> CSceneBase::GetCamerasFromScene() {
		std::vector<CCamera*> pCameras;

		if (pRootActor) {
			std::vector<CActorBase*> pActors;
			this->GetActorsRecursiveInClass(pActors, pRootActor, (int)ClassType::Camera);

			if (!pActors.empty()) {
				for (const auto pActor : pActors) {
					pCameras.emplace_back(dynamic_cast<CCamera*>(pActor));
				}
			}
		}

		return pCameras;
	}

	// シーン内のライトを全て取得
	std::vector<CLight*> CSceneBase::GetLightsFromScene() {
		std::vector<CLight*> pLights;

		if (pRootActor) {
			std::vector<CActorBase*> pActors;
			this->GetActorsRecursiveInClass(pActors, pRootActor, (int)ClassType::Light);

			if (!pActors.empty()) {
				for (const auto pActor : pActors) {
					pLights.emplace_back(dynamic_cast<CLight*>(pActor));
				}
			}
		}

		return pLights;
	}

	// 指定アクター名のアクターを再帰検索して取得
	CActorBase* CSceneBase::GetActorRecursiveInName(CActorBase* pRootActor, std::string& actorName) {
		if (pRootActor) {
			for (const auto pChild : pRootActor->GetChildActorList()) {
				if (pChild->GetActorName() == actorName) {
					return pChild;
				}

				this->GetActorRecursiveInName(pChild, actorName);
			}
		}

		return nullptr;
	}

	// 指定クラスのアクターを再帰検索して取得
	CActorBase* CSceneBase::GetActorRecursiveInClass(CActorBase* pRootActor, int classID) {
		if (pRootActor) {
			for (const auto pChild : pRootActor->GetChildActorList()) {
				if (pChild->GetClassID() == classID) {
					return pChild;
				}

				this->GetActorRecursiveInClass(pChild, classID);
			}
		}

		return nullptr;
	}

	// 指定アクター名のアクター全てを再帰検索して取得
	void CSceneBase::GetActorsRecursiveInName(std::vector<CActorBase*>& pActors, CActorBase* pRootActor, std::string& actorName) {
		if (pRootActor) {
			for (const auto pChild : pRootActor->GetChildActorList()) {
				if (pChild->GetActorName() == actorName) {
					pActors.emplace_back(pChild);
				}

				this->GetActorsRecursiveInName(pActors, pChild, actorName);
			}
		}
	}

	// 指定クラスのアクター全てを再帰検索して取得
	void CSceneBase::GetActorsRecursiveInClass(std::vector<CActorBase*>& pActors, CActorBase* pRootActor, int classID) {
		if (pRootActor) {
			for (const auto pChild : pRootActor->GetChildActorList()) {
				if (pChild->GetClassID() == classID) {
					pActors.emplace_back(pChild);
				}

				this->GetActorsRecursiveInClass(pActors, pChild, classID);
			}
		}
	}

} // namespace