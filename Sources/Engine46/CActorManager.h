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
        UINT lightCount;

        classCount() :
            allCount(0),
            rootCount(0),
            cameraCount(0),
            spriteCount(0),
            lightCount(0)
        {}
    };

    class CActorManager {
    private:
        std::vector<std::unique_ptr<CActorBase>>    m_pVecActor;
        std::vector<std::unique_ptr<CLight>>        m_pVecLight;

        CRendererBase* pRenderer;

        classCount                                  m_classCount;

    public:
        explicit CActorManager(CRendererBase* pRenderer);
        ~CActorManager();

        CActorBase* CreateActor(int actorType);
        CLight* CreateLight(int lightType);

        void AddActorToVec(std::unique_ptr<CActorBase>& pActor) { m_pVecActor.emplace_back(std::move(pActor)); }
        void AddLightToVec(std::unique_ptr<CLight>& pLight) { m_pVecLight.emplace_back(std::move(pLight)); }

        CActorBase* GetActorFromActorName(const char* name);

        bool SaveActor();
        bool LoadActor();

    private:
        void ConnectActor();

    };
} // namespace

#endif