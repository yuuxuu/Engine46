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
#include "MyInspectorWidget.h"

#include "Engine46/CActor.h"

#include "ui_Engine46Editor.h"

class Engine46Editor : public QMainWindow {
private:
    Ui::Engine46Editor ui;

    std::unique_ptr<MyRenderWidget> pRenderWidget;
    std::unique_ptr<MyInspectorWidget> pInspectorWidget;
public:
    Engine46Editor(QWidget* parent = nullptr);
    ~Engine46Editor();

    void InitializeEditor();

    void InitializeSceneTreeView();

    void InitializeFileTreeView();

    QWidget* GetRenderWidget() const { return pRenderWidget.get(); }

public slots:
    void SelectActorForMouseLeftPress(const QPoint& point);

    void SelectActorForSceneTreeView(const QModelIndex& index);
};
