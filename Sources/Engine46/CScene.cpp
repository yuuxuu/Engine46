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
	CSceneBase::CSceneBase(const SceneType sceneType, const char* sceneName) :
		pParentScene(nullptr),
		m_SceneType(sceneType),
		m_SceneID(g_SceneCount++),
		m_SceneName()
	{
		int strSize = strlen(sceneName) + 1;
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
			DATARECORD(DATATYPE::TYPE_VAL, offsetof(CSceneBase, m_SceneType), sizeof(m_SceneType)),
			DATARECORD(DATATYPE::TYPE_VAL, offsetof(CSceneBase, m_SceneID), sizeof(m_SceneID)),
			DATARECORD(DATATYPE::TYPE_VAL, offsetof(CSceneBase, m_SceneName), sizeof(m_SceneName)),
			DATARECORD(DATATYPE::TYPE_VAL, offsetof(CSceneBase, pParentScene), sizeof(pParentScene)),
			DATARECORD(DATATYPE::TYPE_VAL, offsetof(CSceneBase, pChiledSceneList), sizeof(pChiledSceneList)),
			DATARECORD(DATATYPE::TYPE_END, 0, 0)
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
		for (auto& records : vecDataRecord) {
			if (records.dataType == DATATYPE::TYPE_END) break;

			if (records.dataType == DATATYPE::TYPE_STR) {
				for (auto& strRecords : vecStrDataRecord) {
					if (records.offset == strRecords.offset) {
						int size = (int)strlen(strRecords.pStr.get()) + 1;
						ofs.write((char*)&size, sizeof(int));

						ofs.write((char*)&strRecords.pStr, size);
					}
				}
			}
			else if (records.dataType == DATATYPE::TYPE_PTR) {
				int elem = -1;
				void* p = (void*)*((__int64*)(char*)this + records.offset);
				if (p) {
					//elem = GetElementNumberFromPoint(p);
				}
				ofs.write((char*)&elem, sizeof(int));
			}
			else {
				ofs.write((char*)this + records.offset, records.size);
			}
		}

		return true;
	}

	// シーン読み込み
	bool CSceneBase::Load(std::ifstream& ifs) {
		for (auto& records : vecDataRecord) {
			if (records.dataType == DATATYPE::TYPE_END) break;

			if (records.dataType == DATATYPE::TYPE_STR) {
				for (auto& strRecords : vecStrDataRecord) {
					if (records.offset == strRecords.offset) {
						int size = 0;
						ifs.read((char*)&size, sizeof(int));

						std::string str;
						ifs.read((char*)&str, size);

						strRecords.pStr.reset(new char[size]);
						str.copy(strRecords.pStr.get(), size);
					}
				}
			}
			else if (records.dataType == DATATYPE::TYPE_PTR) {
				size_t elem = 0;
				ifs.read((char*)&elem, sizeof(int));

				std::cout << elem << std::endl;

				void* p = (void*)*(__int64*)((char*)this + records.offset);
				p = reinterpret_cast<__int64*>(elem);
			}
			else {
				ifs.read((char*)this + records.offset, records.size);
			}
		}

		return true;
	}

} // namespace