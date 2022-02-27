/**
 * @file CSceneManager.h
 * @brief
 * @author 木村優
 * @date 2021/05/01
 */

#pragma once

#ifndef _CSCENE_MANAGER_H_
#define _CSCENE_MANAGER_H_

namespace Engine46 {

    // 前方宣言
    class CSceneBase;

    class CSceneManager {
    private:
        std::map<std::string, std::unique_ptr<CSceneBase>>    m_pMapScene;

    private:
        CSceneManager();
        ~CSceneManager();

    public:
        static CSceneManager& GetSceneManager() {
            static CSceneManager sceneManager;
            return sceneManager;
        }

        CSceneBase* CreateScene(const std::string& sceneName = "");

        void AddSceneToMap(const std::string& name, std::unique_ptr<CSceneBase>& pScene);

        CSceneBase* GetSceneFromMap(const std::string& name);
    };

} // namespace

#endif