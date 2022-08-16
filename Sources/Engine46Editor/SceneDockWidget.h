/**
 * @file SceneDockWidget.h
 * @brief
 * @author 木村優
 * @date 2022/03/23
 */

#pragma once

#include <qdockwidget.h>

#include "MyStandardItem.h"

#include "ui_SceneDockWidget.h"

class SceneDockWidget : public QDockWidget {
public:
    Ui::SceneDockWidget ui;

private:
    MyStandardItem* pSelectItem;

public:
    SceneDockWidget(QWidget* parent = nullptr);
    ~SceneDockWidget();

    void Initialize();
    void AddItemToTreeView(Engine46::CActorBase* pActor);

    void UpdateItemText(const QString& text);

public slots:
    void OnContextMenu(const QPoint& point);

    void CreateActor(QAction* pActiron);
    void CreateLight(QAction* pActtion);

    void SelectActor(const QModelIndex& index);
    void SelectItem(Engine46::CActorBase* pActor);
};

