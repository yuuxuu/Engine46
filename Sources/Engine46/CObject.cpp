/**
 * @file CObject.cpp
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#include "CObject.h"
#include "utility.h"

namespace Engine46 {

	static UINT g_ObjectCount = 0;

	std::vector<DATARECORD> CObject::m_vecDataRecords = {
		DATARECORD(DATATYPE::TYPE_VAL, offsetof(CObject, m_ClassID), sizeof(m_ClassID)),
		DATARECORD(DATATYPE::TYPE_PTR, offsetof(CObject, pParent), sizeof(pParent)),
		DATARECORD(DATATYPE::TYPE_PTR, offsetof(CObject, pChiled), sizeof(pChiled)),
		DATARECORD(DATATYPE::TYPE_VAL, offsetof(CObject, m_ObjectID), sizeof(m_ObjectID)),
		DATARECORD(DATATYPE::TYPE_STR, offsetof(CObject, m_Name), sizeof(m_Name)),
		DATARECORD(DATATYPE::TYPE_VAL, offsetof(CObject, m_Transform), sizeof(m_Transform)),
		DATARECORD(DATATYPE::TYPE_END, 0, 0)
	};

	// コンストラクタ
	CObject::CObject() :
		pParent(nullptr),
		pChiled(nullptr),
		m_ClassID(0),
		m_ObjectID(g_ObjectCount++),
		m_Name(),
		m_Transform(Transform())
	{
		this->Initilaize();
	}

	// コンストラクタ
	CObject::CObject(UINT id, const char* name, const Transform transform) :
		pParent(nullptr),
		pChiled(nullptr),
		m_ClassID(id),
		m_ObjectID(g_ObjectCount++),
		m_Name(),
		m_Transform(transform.pos, transform.rotation, transform.scale)
	{
		this->Initilaize(name);
	}

	// デストラクタ
	CObject::~CObject()
	{}

	// 初期化
	void CObject::Initilaize(const char* pStr) {
		std::string name;
		if (pStr) {
			name = pStr;
		}
		else {
			name = "Object_" + std::to_string(m_ObjectID);
		}
		int size = (int)name.size() + 1;

		m_Name.reset(new char[size]);
		strcpy_s(m_Name.get(), size, name.c_str());

		m_vecStrDataRecords = {
			STR_DATARECORD(offsetof(CObject, m_Name), m_Name),
		};
	}

	// 更新
	void CObject::Update() {
		if (pChiled) {
			pChiled->Update();
		}
	}

	// 描画
	void CObject::Draw() {
		if (pChiled) {
			pChiled->Draw();
		}
	}

	// オブジェクトのメンバオフセットに合った文字列取得
	const char* CObject::GetStrFromMemberVariable(int offset) {
		for (auto& record : m_vecStrDataRecords) {
			if (record.offset == offset) {
				return record.pStr.get();
			}
		}

		return nullptr;
	}

	// オブジェクトのメンバオフセットに合った文字列設定
	void CObject::SetStrToMemberVariable(const char* pStr, int strSize, int offset) {
		for (auto& record : m_vecStrDataRecords) {
			if (record.offset == offset) {
				record.pStr.reset(new char[strSize]);
				strcpy_s(record.pStr.get(), strSize, pStr);
			}
		}
	}

} // namespace