/**
 * @file CScene.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CScene.h"

namespace Engine46 {

	static int g_SceneCount = 0;

	// コンストラクタ
	CSceneBase::CSceneBase(const SceneType sceneType) :
		pParentScene(nullptr),
		m_parentSceneID(-1),
		m_SceneType(sceneType),
		m_SceneID(g_SceneCount++),
		m_SceneName()
	{
		std::string str = "Scene_" + std::to_string(m_SceneID);
		int size = (int)str.size() + 1;
		m_SceneName.reset(new char[size]);
		str.resize(size);
		str.copy(m_SceneName.get(), size);

		this->Initialize();
	}

	// コンストラクタ
	CSceneBase::CSceneBase(const SceneType sceneType, const char* sceneName) :
		pParentScene(nullptr),
		m_parentSceneID(-1),
		m_SceneType(sceneType),
		m_SceneID(g_SceneCount++),
		m_SceneName()
	{
		int strSize = (int)strlen(sceneName) + 1;
		m_SceneName.reset(new char[strSize]);
		strcpy_s(m_SceneName.get(), strSize, sceneName);

		this->Initialize();
	}

	// デストラクタ
	CSceneBase::~CSceneBase() 
	{}

	//初期化
	void CSceneBase::Initialize() {
		vecDataRecord = {
			DATARECORD(DATATYPE::TYPE_VAL,  offsetof(CSceneBase, m_SceneType), sizeof(m_SceneType)),
			DATARECORD(DATATYPE::TYPE_VAL,  offsetof(CSceneBase, m_SceneID), sizeof(m_SceneID)),
			DATARECORD(DATATYPE::TYPE_STR,  offsetof(CSceneBase, m_SceneName), sizeof(m_SceneName)),
			DATARECORD(DATATYPE::TYPE_PTR,  offsetof(CSceneBase, pParentScene), sizeof(pParentScene)),
			DATARECORD(DATATYPE::TYPE_LIST, offsetof(CSceneBase, pChiledSceneList), sizeof(pChiledSceneList)),
			DATARECORD(DATATYPE::TYPE_END,  0, 0)
		};

		vecStrDataRecord = {
			STR_DATARECORD(offsetof(CSceneBase, m_SceneName), m_SceneName),
		};
	}

	// 更新
	void CSceneBase::Update() {

	}

	// 描画
	void CSceneBase::Draw() {

	}

	// シーン出力
	bool CSceneBase::Save(std::ofstream& ofs) {
		for (const auto& records : vecDataRecord) {
			if (records.dataType == DATATYPE::TYPE_END) break;

			if (records.dataType == DATATYPE::TYPE_STR) {
				for (auto& strRecords : vecStrDataRecord) {
					if (records.offset == strRecords.offset) {
						int size = (int)strlen(strRecords.pStr.get()) + 1;
						ofs.write((char*)&size, sizeof(int));

						ofs.write(strRecords.pStr.get(), size);
					}
				}
			}
			else if (records.dataType == DATATYPE::TYPE_PTR) {
				int id = -1;
				if (pParentScene) {
					id = pParentScene->m_parentSceneID;
				}
				ofs.write((char*)&id, sizeof(int));
			}
			else if (records.dataType == DATATYPE::TYPE_LIST) {
				int listSize = (int)pChiledSceneList.size();
				ofs.write((char*)&listSize, sizeof(int));

				for (const auto& chiled : pChiledSceneList) {
					ofs.write((char*)&chiled->m_SceneID, sizeof(int));
				}
			}
			else {
				ofs.write((char*)this + records.offset, records.size);
			}
		}

		return true;
	}

	// シーン読み込み
	bool CSceneBase::Load(std::ifstream& ifs) {
		for (const auto& records : vecDataRecord) {
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

				m_parentSceneID = id;
			}
			else if (records.dataType == DATATYPE::TYPE_LIST) {
				int listSize = 0;
				ifs.read((char*)&listSize, sizeof(int));

				for (auto i = 0; i < listSize; ++i) {
					int id = -1;
					ifs.read((char*)&id, sizeof(int));

					m_chiledSceneIDList.emplace_back(id);
				}
			}
			else {
				ifs.read((char*)this + records.offset, records.size);
			}
		}

		return true;
	}

} // namespace