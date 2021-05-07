﻿/**
 * @file CObject.cpp
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#include "CActor.h"

namespace Engine46 {

	static UINT g_ActorCount = 0;

	// コンストラクタ
	CActorBase::CActorBase() :
		m_ClassID(0),
		m_ActorID(g_ActorCount++),
		m_ActorName(),
		m_Transform(Transform()),
		pParentActor(nullptr),
		m_parentActorID(-1)
	{
		std::string str = "Object_" + std::to_string(m_ActorID);
		int size = (int)str.size() + 1;
		m_ActorName.reset(new char[size]);
		str.resize(size);
		str.copy(m_ActorName.get(), size);
	}

	// コンストラクタ
	CActorBase::CActorBase(const UINT id, const char* name, const Transform transform) :
		m_ClassID(id),
		m_ActorID(g_ActorCount++),
		m_ActorName(),
		m_Transform(transform),
		pParentActor(nullptr),
		m_parentActorID(-1)
	{
		std::string str = name + std::string('_' + std::to_string(m_ActorID));
		int size = (int)str.size() + 1;
		m_ActorName.reset(new char[size]);
		str.resize(size);
		str.copy(m_ActorName.get(), size);
	}

	// デストラクタ
	CActorBase::~CActorBase()
	{}

	// 初期化
	void CActorBase::Initialize() {

		vecDataRecords.clear();

		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CActorBase, m_ClassID), sizeof(m_ClassID)));
		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CActorBase, m_ActorID), sizeof(m_ActorID)));
		vecDataRecords.emplace_back(std::make_unique<CStrDataRecord>(offsetof(CActorBase, m_ActorName), m_ActorName));
		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CActorBase, m_Transform), sizeof(m_Transform)));
		vecDataRecords.emplace_back(std::make_unique<CPtrDataRecord>(m_parentActorID));
		vecDataRecords.emplace_back(std::make_unique<CListDataRecord>(m_chiledActorIDList));
	}

	// 更新
	void CActorBase::Update() {
		for(auto& chiled : pChiledActorList) {
			chiled->Update();
		}
	}

	// 描画
	void CActorBase::Draw() {

		if (m_pMaterial) {
			m_pMaterial->Set(1);
		}

		if (m_pMesh) {
			m_pMesh->Draw();
		}

		for (auto& chiled : pChiledActorList) {
			chiled->Draw();
		}
	}

	// オブジェクトを保存
	bool CActorBase::Save(std::ofstream& ofs) {
		for (auto& record : vecDataRecords) {
			record->WriteData(ofs, (char*)this);
		}

		return true;
	}

	// オブジェクト読み込み
	bool CActorBase::Load(std::ifstream& ifs) {
		for (auto& record : vecDataRecords) {
			if (&record == &vecDataRecords[0]) continue;

			record->ReadData(ifs, (char*)this);
		}

		return true;
	}

	// メッシュ作成
	void CActorBase::CreateMesh(CDX11Renderer* pRenderer) {
		if (!m_pMesh) {
			m_pMesh = std::make_unique<CDX11Mesh>(pRenderer);
		}
	}

	// マテリアル作成
	void CActorBase::CreateMaterial(CDX11Renderer* pRenderer) {
		if (!m_pMaterial) {
			m_pMaterial = std::make_unique<CDX11Material>(pRenderer);
		}
	}

	// 親アクターを接続
	void CActorBase::ConnectParentActor(CActorBase* pParentActor) {
		
		if (this->pParentActor == pParentActor) return;
		
		this->pParentActor = pParentActor;

		if (pParentActor) {
			m_parentActorID = pParentActor->m_ActorID;
		}
		else {
			m_parentActorID = -1;
		}
	}

	// 子アクターを追加
	void CActorBase::AddChiledActorList(CActorBase* pChiledActor) {
		if (pChiledActor) {
			auto it = std::find(m_chiledActorIDList.begin(), m_chiledActorIDList.end(), pChiledActor->m_ActorID);

			if (it == m_chiledActorIDList.end()) {
				pChiledActor->ConnectParentActor(this);

				pChiledActorList.emplace_back(pChiledActor);

				m_chiledActorIDList.emplace_back(pChiledActor->m_ActorID);
			}
		}
	}

} // namespace