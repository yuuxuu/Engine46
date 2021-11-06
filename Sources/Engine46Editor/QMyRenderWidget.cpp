/**
 * @file QMyRenderWidget.cpp
 * @brief
 * @author 木村優
 * @date 2021/07/02
 */

#include "QMyRenderWidget.h"

#include "Engine46/CGameSystem.h"
#include "Engine46/CInput.h"

#include <QMouseEvent>

 // コンストラクタ
QMyRenderWidget::QMyRenderWidget(QWidget* parent) :
    QWidget(parent)
{
    resize(QSize(1280, 720));
}

// デストラクタ
QMyRenderWidget::~QMyRenderWidget()
{}

// マウス押下イベント
void QMyRenderWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MouseButton::LeftButton) {
        emit MouseLeftPress(event->pos());
    }
}

// ウェジットにマウスが入った際のイベント
void QMyRenderWidget::enterEvent(QEvent* event) {
    Engine46::CGameSystem::GetGameSystem().GetInput()->ChangeUpdateState(true);
}

// ウェジットからマウスが出た際のイベント
void QMyRenderWidget::leaveEvent(QEvent* event) {
    Engine46::CGameSystem::GetGameSystem().GetInput()->ChangeUpdateState(false);
}