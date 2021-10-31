/**
 * @file Engine46ActorEditor.h
 * @brief
 * @author 木村優
 * @date 2021/06/26
 */

#pragma once

#include <QWidget>

#include "../Engine46/CActor.h"

#include "ui_Engine46ActorEditor.h"

class Engine46ActorEditor : public QWidget {
public:
    Engine46ActorEditor(QWidget* parent = Q_NULLPTR);
    ~Engine46ActorEditor();

    void SelectActorReflectToEditor();
    void EditorReflectToSelectActor();

    void ChangeValueReflectToName(const QString& string);
public:
    Ui::Engine46ActorEditor ui;

private:
    Engine46::CActorBase* pSelectActor = nullptr;

public slots:
    void SetSelectActor(const QModelIndex& index);

    void ChangeValueReflectToPos(double value);
    void ChangeValueReflectToRotation(double value);
    void ChangeValueReflectToScale(double value);
};

