/**
 * @file IObject.h
 * @brief
 * @author 木村優
 * @date 2021/05/01
 */

#pragma once

#ifndef _IOBJECT_H_
#define _IOBJECT_H_

namespace Engine46 {

    class IObject {
    protected:
        IObject() {}
        virtual ~IObject() {}

        virtual void Initialize() = 0;
        virtual void Update() = 0;
        virtual void Draw() = 0;
    };

} // namespace

#endif
