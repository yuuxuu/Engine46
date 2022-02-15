/**
 * @file CSprite.h
 * @brief
 * @author 木村優
 * @date 2001/05/06
 */

#pragma once

#ifndef _CSPRITE_H_
#define _CSPRITE_H_

#include "CActor.h"

namespace Engine46 {

    class CSprite : public CActorBase {
    private:

    public:
        CSprite(const std::string& actorName);
        ~CSprite();

        void Update() override;
    };

} // namespace

#endif