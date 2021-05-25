/**
 * @file CScene.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CScene.h"
#include "CActor.h"

namespace Engine46 {

	static int g_SceneCount = 0;

	// コンストラクタ
	CSceneBase::CSceneBase() :
		pParentScene(nullptr),
		m_parentSceneID(-1),
		m_SceneID(g_SceneCount++),
		m_SceneName()
	{
		std::string str = "Scene_" + std::to_string(m_SceneID);
		int size = (int)str.size() + 1;
		m_SceneName.reset(new char[size]);
		str.resize(size);
		str.copy(m_SceneName.get(), size);
	}

	// コンストラクタ
	CSceneBase::CSceneBase(const char* sceneName) :
		pParentScene(nullptr),
		m_parentSceneID(-1),
		m_SceneID(g_SceneCount++),
		m_SceneName()
	{
		int strSize = (int)strlen(sceneName) + 1;
		m_SceneName.reset(new char[strSize]);
		strcpy_s(m_SceneName.get(), strSize, sceneName);
	}

	// デストラクタ
	CSceneBase::~CSceneBase() 
	{}

	//初期化
	void CSceneBase::Initialize() {

		vecDataRecords.clear();

		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CSceneBase, m_SceneID), sizeof(m_SceneID)));
		vecDataRecords.emplace_back(std::make_unique<CStrDataRecord>(offsetof(CSceneBase, m_SceneName), m_SceneName));
		vecDataRecords.emplace_back(std::make_unique<CPtrDataRecord>(m_parentSceneID));
		vecDataRecords.emplace_back(std::make_unique<CListDataRecord>(m_chiledSceneIDList));
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
			records->WriteData(ofs, (char*)this);
		}

		return true;
	}

	// シーン読み込み
	bool CSceneBase::Load(std::ifstream& ifs) {

		for (auto& records : vecDataRecords) {
			if (&records == &vecDataRecords[0]) continue;

			records->ReadData(ifs, (char*)this);
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

} // namespace