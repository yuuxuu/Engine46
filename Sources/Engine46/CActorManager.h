/**
 * @file CActorManager.h
 * @brief
 * @author 木村優
 * @date 2019/01/02
 */

#pragma once

#ifndef _CACTOR_MANAGER_H_
#define _CACTOR_MANAGER_H_

#include "CLight.h"

namespace Engine46 {

    // 前方宣言
    class CActorBase;
    class CLight;
    class CRendererBase;

    struct classCount {
        UINT allCount;
        UINT rootCount;
        UINT cameraCount;
        UINT spriteCount;
        UINT boxCount;
        UINT charctorCount;
        UINT lightCount;

        classCount() :
            allCount(0),
            rootCount(0),
            cameraCount(0),
            spriteCount(0),
            boxCount(0),
            charctorCount(0),
            lightCount(0)
        {}
    };

    class CActorManager {
    private:
        std::map<std::string, std::unique_ptr<CActorBase>>    m_pMapActor;
        std::map<std::string, std::unique_ptr<CLight>>        m_pMapLight;

        CRendererBase* pRenderer;

        classCount                                  m_classCount;

    public:
        explicit CActorManager(CRendererBase* pRenderer);
        ~CActorManager();

        CActorBase* CreateActor(ActorType actorType);
        CLight* CreateLight(LightType lightType);

        void AddActorFromMap(const char* name, std::unique_ptr<CActorBase>& pActor);
        CActorBase* GetActorFromMap(const char* name);

        void AddLightFromMap(const char* name, std::unique_ptr<CLight>& pLight);
        CActorBase* GetLightFromMap(const char* name);

        bool SaveActor();
        bool LoadActor();

    private:
        void ConnectActor();

    };
} // namespace

#endif