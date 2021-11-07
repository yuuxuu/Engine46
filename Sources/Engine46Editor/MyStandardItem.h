/**
 * @file MyStandardItem.h
 * @brief
 * @author 木村優
 * @date 2021/11/07
 */

#pragma once

#include <QStandardItem>

#include "Engine46/CActor.h"

class MyStandardItem : public QStandardItem {
private:
    Engine46::CActorBase* pActor;

public:
    explicit MyStandardItem(Engine46::CActorBase* pActor);
    ~MyStandardItem();

    Engine46::CActorBase* GetActor() const { return pActor; }
};
