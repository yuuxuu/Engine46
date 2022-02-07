/**
 * @file CScene.h
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#pragma once

#ifndef _CSCENE_H_
#define _CSCENE_H_

#include "IObject.h"
#include "math.h"

namespace Engine46 {

    // 前方宣言
    class CDataRecordBase;
    class CActorBase;
    class CCamera;
    class CLight;
    class CSprite;
    class CParticleEmitter;

    class CSceneBase {
    protected:
        std::vector<CDataRecordBase>    vecDataRecords;

        CActorBase*                     pRootActor;

        std::string                     m_sceneName;

        UINT                            m_sceneID;

    public:
        CSceneBase();
        explicit CSceneBase(const char* sceneName);
        ~CSceneBase();

        void Initialize();
        void Update();
        void Draw();

        bool Save(std::ofstream& ofs);
        bool Load(std::ifstream& ifs);

        void AddActorToScene(CActorBase* pActor);

        CActorBase* GetActorFromMap(std::string& actorName);

        CActorBase* GetMouseSelectActorFromScene(VECTOR2& screenSize, VECTOR2& mousePos);

        CActorBase* GetSkyDomeFromScene();
        CCamera* GetCameraFromScene();
        CLight* GetLightFromScene();

        std::vector<CActorBase*> GetActorsFromScene();
        std::vector<CCamera*> GetCamerasFromScene();
        std::vector<CLight*> GetLightsFromScene();
        std::vector<CParticleEmitter*> GetParticleEmittersFromScene();

        void SetRootActor(CActorBase* pRootActor) { this->pRootActor = pRootActor; }
        CActorBase* GetRootActor() const { return pRootActor; }

        void SetSceneName(const std::string& sceneName) { m_sceneName = sceneName; }
        std::string GetSceneName() const { return m_sceneName.c_str(); }

        void SetSceneID(const int id) { m_sceneID = id; }

    private:
        CActorBase* GetActorRecursiveInName(CActorBase* pRootActor, std::string& actorName);
        CActorBase* GetActorRecursiveInActor(CActorBase* pRootActor, int actorType);

        void GetActorsRecursiveInName(std::vector<CActorBase*>& pActors, CActorBase* pRootActor, std::string& actorName);
        void GetActorsRecursiveInActor(std::vector<CActorBase*>& pActors, CActorBase* pRootActor, int actorType);
    };

} // namespace

#endif
