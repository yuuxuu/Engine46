/**
 * @file TransformWidget.h
 * @brief
 * @author 木村優
 * @date 2022/03/24
 */

#pragma once

#include <QWidget>

#include "ui_TransformWidget.h"

#include "Engine46/math.h"

class TransformWidget : public QWidget {
    Q_OBJECT
private:
    Ui::TransformWidget ui;

public:
    TransformWidget(QWidget* parent = nullptr);
    ~TransformWidget();

    void SetTransform(const Engine46::Transform& transform);

    Engine46::Transform m_transform;

public slots:
    void UpdatePosition(double value);
    void UpdateRotation(double value);
    void UpdateScale(double value);

Q_SIGNALS:
    void UpdateTransformToActor(const Engine46::Transform& transform);
};
