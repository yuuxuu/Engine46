/**
 * @file CScene.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CScene.h"

namespace Engine46 {

	// コンストラクタ
	CSceneBase::CSceneBase(const SceneType type, const std::string& scene) :
		pPrevScene(nullptr),
		pNextScene(nullptr),
		m_SceneType(type),
		m_SceneName(scene)
	{}

	// デストラクタ
	CSceneBase::~CSceneBase() 
	{}

	// 次のシーンへ遷移
	void CSceneBase::TransitionNextScene() {

	}

	// 前のシーンへ遷移
	void CSceneBase::TransitionPrevScene() {

	}

	// シーン出力
	void CSceneBase::SaveScene() {
		
	}

	// シーン読み込み
	void CSceneBase::LoadScene() {

	}

	// シーン解放
	void CSceneBase::ReleaseScene() {

	}

} // namespace