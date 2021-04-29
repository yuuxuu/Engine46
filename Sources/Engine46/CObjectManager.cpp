/**
 * @file CObjectManager.cpp
 * @brief
 * @author 木村優
 * @date 2019/01/02
 */

#include "CObjectManager.h"
#include "CFileSystem.h"

namespace Engine46 {

	// コンストラクタ
	CObjectManager::CObjectManager() :
		m_pObjectListFileName("ObjectListData.bin")
	{}

	// デストラクタ
	CObjectManager::~CObjectManager()
	{}

	// オブジェクト作成
	CObject* CObjectManager::CreateObject(int id) {
		std::unique_ptr<CObject> object = std::make_unique<CObject>();

		CObject* pObject = object.get();

		AddObjectToObjectList(object);

		return pObject;
	}

	// オブジェクト同士の接続
	void CObjectManager::ConnectObject() {
		for (auto& object : m_pObjectList) {
			size_t parent = reinterpret_cast<size_t>(object->GetParentObject());
			if ((int)parent > -1) {
				object->ConnectParent(m_pObjectList[parent].get());
			}

			size_t chiled = reinterpret_cast<size_t>(object->GetChiledObject());
			if ((int)chiled > -1) {
				object->ConnectChiled(m_pObjectList[chiled].get());
			}
		}
	}

	// オブジェクトリストを保存
	bool CObjectManager::SaveObjectList() {

		std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary;
		LPVOID pStr = nullptr;
		size_t bufferSize = 0;

		std::ofstream ofs;
		ofs.open(m_pObjectListFileName, mode);

		if (!ofs.is_open()) return false;

		for (const auto& object : m_pObjectList) {
			DATARECORD* record = object->GetObjectDataRecords();

			for (;;) {
				if (record->dataType == DATATYPE::TYPE_END) break;

				if (record->dataType == DATATYPE::TYPE_STR) {
					const char* str = object->GetStrFromMemberVariable(record->offset);
					int size = (int)strlen(str) + 1;

					ofs.write((char*)&size, sizeof(int));
					ofs.write((char*)str, size);
				}
				else if (record->dataType == DATATYPE::TYPE_PTR) {
					int elem = -1;
					void* p = (void*)*(__int64*)((char*)object.get() + record->offset);
					if (p) {
						elem = GetElementNumberFromPoint(p);
					}
					ofs.write((char*)&elem, sizeof(int));
				}
				else {
					ofs.write((char*)object.get() + record->offset, record->size);
				}

				record++;
			}
		}

		return true;
	}

	// オブジェクトリストを読み込み
	bool CObjectManager::LoadObjectList() {

		CFileSystem fileSystem;
		std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary;
		LPVOID pStr = nullptr;
		size_t bufferSize = 0;

		std::ifstream ifs;

		ifs.open(m_pObjectListFileName, mode);

		if (!ifs.is_open()) return false;

		while (true) {

			int id = 0;
			ifs.read((char*)&id, sizeof(UINT));

			if (ifs.eof()) break;

			CObject* object = CreateObject(id);

			DATARECORD* record = object->GetObjectDataRecords();
			record++;

			for (;;) {
				if (record->dataType == DATATYPE::TYPE_END) break;

				if (record->dataType == DATATYPE::TYPE_STR){
					int size = 0;
					std::string str;

					ifs.read((char*)&size, sizeof(int));
					ifs.read(str.data(), size);
					
					object->SetStrToMemberVariable(str.c_str(), size, record->offset);
				}
				else if (record->dataType == DATATYPE::TYPE_PTR) {
					size_t elem = 0;
					ifs.read((char*)&elem, sizeof(int));

					std::cout << elem << std::endl;

					void*& p = (void*&)*(__int64*)((char*)object + record->offset);
					p = reinterpret_cast<size_t*>(elem);
				}
				else {
					ifs.read((char*)object + record->offset, record->size);
				}

				record++;
			}
		}

		std::cout << m_pObjectListFileName << "を読み込みしました。" << std::endl;

		this->ConnectObject();

		return true;
	}

	// オブジェクトのポインタから要素番号を取得
	int CObjectManager::GetElementNumberFromPoint(void* p){
		int elem = 0;
		for (const auto& object : m_pObjectList) {
			if (object.get() == p) return elem;

			elem++;
		}

		return -1;
	}

} // namespace