﻿/**
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
        UINT charctorCount;
        UINT particeleEmitterCount;
        UINT lightCount;

        classCount() :
            allCount(0),
            rootCount(0),
            cameraCount(0),
            charctorCount(0),
            particeleEmitterCount(0),
            lightCount(0)
        {}
    };

    class CActorManager {
    private:
        std::map<std::string, std::unique_ptr<CActorBase>>      m_pMapActor;
        std::map<std::string, std::unique_ptr<CLight>>          m_pMapLight;

        CRendererBase* pRenderer;

        classCount                                              m_classCount;

    public:
        explicit CActorManager(CRendererBase* pRenderer);
        ~CActorManager();

        CActorBase* CreateActor(ActorType actorType);
        CLight* CreateLight(LightType lightType);

        void AddActorFromMap(const std::string& name, std::unique_ptr<CActorBase>& pActor);
        CActorBase* GetActorFromMap(const std::string& name);

        void AddLightFromMap(const std::string& name, std::unique_ptr<CLight>& pLight);
        CActorBase* GetLightFromMap(const std::string& name);

    private:
        void ConnectActor();


    };
} // namespace

#endif