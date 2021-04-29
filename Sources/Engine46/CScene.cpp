/**
 * @file CScene.cpp
 * @brief
 * @author �ؑ��D
 * @date 2020/05/18
 */

#include "CScene.h"

namespace Engine46 {

	// �R���X�g���N�^
	CSceneBase::CSceneBase(const SceneType type, const std::string& scene) :
		pPrevScene(nullptr),
		pNextScene(nullptr),
		m_SceneType(type),
		m_SceneName(scene)
	{}

	// �f�X�g���N�^
	CSceneBase::~CSceneBase() 
	{}

	// ���̃V�[���֑J��
	void CSceneBase::TransitionNextScene() {

	}

	// �O�̃V�[���֑J��
	void CSceneBase::TransitionPrevScene() {

	}

	// �V�[���o��
	void CSceneBase::SaveScene() {
		
	}

	// �V�[���ǂݍ���
	void CSceneBase::LoadScene() {

	}

	// �V�[�����
	void CSceneBase::ReleaseScene() {

	}

} // namespace