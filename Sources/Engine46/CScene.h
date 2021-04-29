/**
 * @file CScene.h
 * @brief
 * @author –Ø‘º—D
 * @date 2020/05/18
 */

#ifndef _CSCENE_H_
#define _CSCENE_H_

#pragma once

namespace Engine46 {

	enum class SceneType {
		TITLE,
		GAME,
		RESULT,
		LOADING,
		NONE,
	};

	class CSceneBase {
	protected:
		CSceneBase* pPrevScene;
		CSceneBase* pNextScene;

		SceneType	m_SceneType;
		std::string	m_SceneName;


		CSceneBase(const SceneType sceneType, const std::string& sceneName);
		virtual ~CSceneBase();

		virtual void	InitScene() = 0;
		virtual void	UpdateScene() = 0;
		virtual void	DrawScene() = 0;

		void			TransitionNextScene();
		void			TransitionPrevScene();

		void			SaveScene();
		void			LoadScene();
		void			ReleaseScene();

		SceneType		GetSceneType() const { return m_SceneType; }
		void			SetSceneType(const SceneType type) { m_SceneType = type; }
		
		std::string		GetSceneName() const { return m_SceneName; }
		void			SetSceneName(const std::string name) { m_SceneName = name; }
	};

} // namespace

#endif
