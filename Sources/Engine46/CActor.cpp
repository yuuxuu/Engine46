/**
 * @file CObject.cpp
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#include "CActor.h"
#include "utility.h"

namespace Engine46 {

	static UINT g_ActorCount = 0;

	// コンストラクタ
	CActorBase::CActorBase() :
		pParentObject(nullptr),
		m_parentObjectID(0),
		m_ClassID(0),
		m_ObjectID(g_ActorCount++),
		m_Name(),
		m_Transform(Transform())
	{
		std::string str = "Object_" + std::to_string(m_ObjectID);
		int size = (int)str.size() + 1;
		m_Name.reset(new char[size]);
		str.resize(size);
		str.copy(m_Name.get(), size);

		this->Initialize();
	}

	// コンストラクタ
	CActorBase::CActorBase(UINT id, const char* name, const Transform transform) :
		pParentObject(nullptr),
		m_ClassID(id),
		m_ObjectID(g_ActorCount++),
		m_Name(),
		m_Transform(transform.pos, transform.rotation, transform.scale)
	{
		std::string str = name;
		int size = (int)str.size() + 1;
		m_Name.reset(new char[size]);
		str.resize(size);
		str.copy(m_Name.get(), size);

		this->Initialize();
	}

	// デストラクタ
	CActorBase::~CActorBase()
	{}

	// 初期化
	void CActorBase::Initialize() {
		vecDataRecord = {
			DATARECORD(DATATYPE::TYPE_VAL , offsetof(CActorBase, m_ClassID), sizeof(m_ClassID)),
			DATARECORD(DATATYPE::TYPE_PTR , offsetof(CActorBase, pParentObject), sizeof(pParentObject)),
			DATARECORD(DATATYPE::TYPE_LIST, offsetof(CActorBase, pChiledObjectList), sizeof(pChiledObjectList)),
			DATARECORD(DATATYPE::TYPE_VAL , offsetof(CActorBase, m_ObjectID), sizeof(m_ObjectID)),
			DATARECORD(DATATYPE::TYPE_STR , offsetof(CActorBase, m_Name), sizeof(m_Name)),
			DATARECORD(DATATYPE::TYPE_VAL , offsetof(CActorBase, m_Transform), sizeof(m_Transform)),
			DATARECORD(DATATYPE::TYPE_END , 0, 0)
		};

		vecStrDataRecord = {
			STR_DATARECORD(offsetof(CActorBase, m_Name), m_Name),
		};
	}

	// 更新
	void CActorBase::Update() {
		for(auto& chiled : pChiledObjectList) {
			chiled->Update();
		}
	}

	// 描画
	void CActorBase::Draw() {
		for (auto& chiled : pChiledObjectList) {
			chiled->Draw();
		}
	}

	// オブジェクトを保存
	bool CActorBase::Save(std::ofstream& ofs) {
		for (auto& records : vecDataRecord) {
			if (records.dataType == DATATYPE::TYPE_END) break;

			if (records.dataType == DATATYPE::TYPE_STR) {
				for (auto& strRecord : vecStrDataRecord) {
					if (records.offset == strRecord.offset) {
						const char* str = strRecord.pStr.get();
						int size = (int)strlen(str) + 1;

						ofs.write((char*)&size, sizeof(int));
						ofs.write(strRecord.pStr.get(), size);
					}
				}
			}
			else if (records.dataType == DATATYPE::TYPE_PTR) {
				int id = -1;
				if (pParentObject) {
					id = pParentObject->m_ObjectID;
				}
				ofs.write((char*)&id, sizeof(int));
			}
			else if (records.dataType == DATATYPE::TYPE_LIST) {
				int listSize = (int)pChiledObjectList.size();
				ofs.write((char*)&listSize, sizeof(int));
				
				for (const auto& chiled : pChiledObjectList) {
					ofs.write((char*)&chiled->m_ObjectID, sizeof(int));
				}
			}
			else {
				ofs.write((char*)this + records.offset, records.size);
			}
		}

		return true;
	}

	// オブジェクト読み込み
	bool CActorBase::Load(std::ifstream& ifs) {
		for (auto& records : vecDataRecord) {
			if (&records == &vecDataRecord[0]) continue;

			if (records.dataType == DATATYPE::TYPE_END) break;

			if (records.dataType == DATATYPE::TYPE_STR) {
				for (auto& strRecords : vecStrDataRecord) {
					if (records.offset == strRecords.offset) {
						int size = 0;
						ifs.read((char*)&size, sizeof(int));

						std::string str;
						str.resize(size);
						ifs.read(str.data(), size);

						strRecords.pStr.reset(new char[size]);
						str.copy(strRecords.pStr.get(), size);
					}
				}
			}
			else if (records.dataType == DATATYPE::TYPE_PTR) {
				int id = -1;
				ifs.read((char*)&id, sizeof(int));

				m_parentObjectID = id;
			}
			else if (records.dataType == DATATYPE::TYPE_LIST) {
				int listSize = 0;
				ifs.read((char*)&listSize, sizeof(int));

				for (auto i = 0; i < listSize; ++i) {
					int id = -1;
					ifs.read((char*)&id, sizeof(int));

					m_chiledObjectIDList.emplace_back(id);
				}
			}
			else {
				ifs.read((char*)this + records.offset, records.size);
			}
		}

		return true;
	}

} // namespace