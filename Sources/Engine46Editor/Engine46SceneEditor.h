/**
 * @file Engine46SceneEditor.h
 * @brief
 * @author 木村優
 * @date 2021/06/26
 */

#pragma once

#include <QWidget>
#include <QStandardItem>

#include "../Engine46/CActor.h"

#include "ui_Engine46SceneEditor.h"

class Engine46SceneEditor : public QWidget {
public:
    Engine46SceneEditor(QWidget* parent = Q_NULLPTR);
    ~Engine46SceneEditor();

    void InitializeSceneEditor();

    void RecursiveActor(Engine46::CActorBase* pRootActor, QStandardItemModel* pItemModel, QStandardItem* pRootItem);

    void ChangeValueReflectToName(const QString& string);

public:
    Ui::Engine46SceneEditor ui;

private:
    QModelIndex selectIndex;

public slots:
    void SelectItem(const QModelIndex& index);
};

