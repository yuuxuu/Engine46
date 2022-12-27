/**
 * @file MyRenderWidget.h
 * @brief
 * @author 木村優
 * @date 2021/07/02
 */

#pragma once

#include <QWidget>

class MyRenderWidget : public QWidget {
    Q_OBJECT
private:
    bool m_isMouseLeftHold;

    QPoint m_mousePressPos;

public:
    MyRenderWidget(QWidget* parent = Q_NULLPTR);
    ~MyRenderWidget();

private:
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

Q_SIGNALS:
    void MouseLeftPress(const QPoint& point);
};
