/**
 * @file MyStandardItem.cpp
 * @brief
 * @author 木村優
 * @date 2021/11/07
 */

#include "MyStandardItem.h"

MyStandardItem::MyStandardItem(Engine46::CActorBase* pActor) :
    pActor(pActor)
{
    setText(QString::fromStdString(pActor->GetActorName()));
}

MyStandardItem::~MyStandardItem() 
{}
