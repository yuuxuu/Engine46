/**
 * @file IPostEffect.h
 * @brief
 * @author 木村優
 * @date 2021/10/03
 */

#pragma once

#ifndef _IPOST_EFFECT_H
#define _IPOST_EFFECT_H

namespace Engine46 {

    // 前方宣言
    class CRendererBase;

    class IPostEffect {
    protected:
        IPostEffect() {};
        virtual ~IPostEffect() {};

        virtual bool Initialize(CRendererBase* pRenderer, UINT width, UINT height) = 0;
    };

} // namespace

#endif
