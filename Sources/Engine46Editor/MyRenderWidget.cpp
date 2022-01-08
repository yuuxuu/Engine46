/**
 * @file MyRenderWidget.cpp
 * @brief
 * @author 木村優
 * @date 2021/07/02
 */

#include "MyRenderWidget.h"

#include "Engine46/CGameSystem.h"
#include "Engine46/CActor.h"
#include "Engine46/CInput.h"

#include <QMouseEvent>

 // コンストラクタ
MyRenderWidget::MyRenderWidget(QWidget* parent) :
    QWidget(parent),
    m_isMouseLeftHold(false)
{}

// デストラクタ
MyRenderWidget::~MyRenderWidget()
{}

// ウェジットにマウスが入った際のイベント
void MyRenderWidget::enterEvent(QEvent* event) {
    Engine46::CInput* pInput = Engine46::CGameSystem::GetGameSystem().GetInput();
    if (!pInput) return;

    pInput->ChangeMouseUpdateState(true);
    pInput->ChangeKeyUpdateState(true);
}

// ウェジットからマウスが出た際のイベント
void MyRenderWidget::leaveEvent(QEvent* event) {
    Engine46::CInput* pInput = Engine46::CGameSystem::GetGameSystem().GetInput();
    if (!pInput) return;

    pInput->ChangeMouseUpdateState(false);
    pInput->ChangeKeyUpdateState(false);

    m_isMouseLeftHold = false;
}

// マウス押下イベント
void MyRenderWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MouseButton::LeftButton) {
        emit MouseLeftPress(event->pos());

        m_mousePressPos = event->pos();

        m_isMouseLeftHold = true;
    }
}

// マウスリリースイベント
void MyRenderWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::MouseButton::LeftButton) {

        m_mousePressPos = QPoint(0, 0);

        m_isMouseLeftHold = false;
    }
}

// マウス移動イベント
void MyRenderWidget::mouseMoveEvent(QMouseEvent* event) {
    if (m_isMouseLeftHold) {
        QPoint diffPos;
        diffPos.setX(m_mousePressPos.x() - event->pos().x());
        diffPos.setY(m_mousePressPos.y() - event->pos().y());

        Engine46::CInput* pInput = Engine46::CGameSystem::GetGameSystem().GetInput();
        if (!pInput) return;

        float length = std::sqrtf(diffPos.x() * diffPos.x() + diffPos.y() * diffPos.y());
        if (fabsf(length) > 3.0f) {
            pInput->ChangeMouseUpdateState(true);
        }
        else {
            pInput->ChangeMouseUpdateState(false);
        }
    }
}
