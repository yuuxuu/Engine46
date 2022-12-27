/**
 * @file MyInspectorWidget.h
 * @brief
 * @author 木村優
 * @date 2021/06/26
 */

#pragma once

#include <QWidget>

#include "ui_InspectorWidget.h"

#include "Engine46/CActor.h"

class MyInspectorWidget : public QWidget {
public:
    Ui::InspectorWidget ui;

private:
    Engine46::CActorBase* pSelectActor;

public:
    MyInspectorWidget(QWidget* parent = Q_NULLPTR);
    ~MyInspectorWidget();

    void SetSelectActor(Engine46::CActorBase* pActor);

public slots:
    void UpdateActorVisible(bool isVisible);
    void UpdateActorName(const QString& name);
    void UpdateActorPosition(double value);
    void UpdateActorRotation(double value);
    void UpdateActorScale(double value);
};
