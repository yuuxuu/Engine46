﻿/**
 * @file CScene.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CScene.h"
#include "CDataRecord.h"
#include "CActor.h"

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

	// アクターの子アクターを再帰検索
	CActorBase* CSceneBase::RecursiveActor(CActorBase* pRootActor, std::string& actorName) {
		if (pRootActor) {
			for (const auto& pChild : pRootActor->GetChildActorList()) {
				if (pChild->GetActorName() == actorName) {
					return pChild;
				}

				this->RecursiveActor(pChild, actorName);
			}
		}

		return nullptr;
	}

	// アクター名でアクターを取得
	CActorBase* CSceneBase::GetActorFromActorName(std::string& actorName) {
		if (pRootActor) {
			if (pRootActor->GetActorName() == actorName) {
				return pRootActor;
			}
			return this->RecursiveActor(pRootActor, actorName);
		}

		return nullptr;
	}

} // namespace