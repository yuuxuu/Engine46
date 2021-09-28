/**
 * @file QMyRenderWidget.cpp
 * @brief
 * @author 木村優
 * @date 2021/07/02
 */

#include "QMyRenderWidget.h"

#include "Engine46/CGameSystem.h"
#include "Engine46/CInput.h"

 // コンストラクタ
QMyRenderWidget::QMyRenderWidget(QWidget* parent) :
    QWidget(parent)
{}

// デストラクタ
QMyRenderWidget::~QMyRenderWidget()
{}

// ウェジットにマウスが入った際のイベント
void QMyRenderWidget::enterEvent(QEvent* event) {
    Engine46::CGameSystem::GetGameSystem().GetInput()->ChangeUpdateState(true);
}

// ウェジットからマウスが出た際のイベント
void QMyRenderWidget::leaveEvent(QEvent* event) {
    Engine46::CGameSystem::GetGameSystem().GetInput()->ChangeUpdateState(false);
}