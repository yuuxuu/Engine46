/**
 * @file Engine46MainEditor.h
 * @brief
 * @author 木村優
 * @date 2021/06/26
 */

#pragma once

#include <QMainWindow>
#include <QTreeView>

#include "MyRenderWidget.h"
#include "SceneDockWidget.h"
#include "InspectorDockWidget.h"
#include "FileDockWidget.h"
#include "MyStandardItem.h"

#include "ui_Engine46Editor.h"

class Engine46Editor : public QMainWindow {
public:
    Ui::Engine46Editor ui;
private:

    std::unique_ptr<MyRenderWidget> pRenderWidget;
    std::unique_ptr<SceneDockWidget> pSceneDockWidget;
    std::unique_ptr<InspectorDockWidget> pInspectorDockWidget;
    std::unique_ptr<FileDockWidget> pFileDockWidget;

public:
    Engine46Editor(QWidget* parent = nullptr);
    ~Engine46Editor();

    void InitializeEditor();

    void SetSelectActor(Engine46::CActorBase* pActor);
    void UpdateActorName(const QString& name);

    QWidget* GetRenderWidget() const { return pRenderWidget.get(); }

public slots:
    void SelectActorForMouseLeftPress(const QPoint& point);
};
