/**
 * @file CObjectManager.cpp
 * @brief
 * @author 木村優
 * @date 2019/01/02
 */

#include "CActorManager.h"
#include "CFileSystem.h"

namespace Engine46 {

	// コンストラクタ
	CActorManager::CActorManager() :
		m_pActorListFileName("ObjectListData.bin")
	{}

	// デストラクタ
	CActorManager::~CActorManager()
	{}

	// オブジェクト作成
	CActorBase* CActorManager::CreateActor(int id) {
		std::unique_ptr<CActorBase> object = std::make_unique<CActorBase>();

		CActorBase* pObject = object.get();

		AddActorToActorList(object);

		return pObject;
	}

	// オブジェクト同士の接続
	void CActorManager::ConnectActor() {
		for (auto& object : m_pActorList) {
			int id = object->GetParentObjectID();
			if (id > -1) {
				object->ConnectParentObject(m_pActorList[id].get());
			}

			for (auto id : object->GetChiledObjectIDList()) {
				object->AddChiledObjectList(m_pActorList[id].get());
			}
		}
	}

	// オブジェクトリストを保存
	bool CActorManager::SaveActorList() {

		std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary;

		std::ofstream ofs;
		ofs.open(m_pActorListFileName, mode);

		if (!ofs.is_open()) return false;

		for (const auto& object : m_pActorList) {
			object->Save(ofs);
		}

		return true;
	}

	// オブジェクトリストを読み込み
	bool CActorManager::LoadActorList() {

		std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary;

		std::ifstream ifs;
		ifs.open(m_pActorListFileName, mode);

		if (!ifs.is_open()) return false;

		while (true) {

			int id = -1;
			ifs.read((char*)&id, sizeof(int));

			if (ifs.eof()) break;

			CActorBase* object = CreateActor(id);

			object->Load(ifs);
		}

		std::cout << m_pActorListFileName << "を読み込みしました。" << std::endl;

		this->ConnectActor();

		return true;
	}

	// オブジェクトのポインタから要素番号を取得
	int CActorManager::GetElementNumberFromPoint(void* p){
		int elem = 0;
		for (const auto& object : m_pActorList) {
			if (object.get() == p) return elem;

			elem++;
		}

		return -1;
	}

} // namespace