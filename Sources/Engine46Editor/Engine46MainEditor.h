/**
 * @file Engine46MainEditor.h
 * @brief
 * @author 木村優
 * @date 2021/06/26
 */

#pragma once

#include <QMainWindow>
#include <QTreeView>

#include "Engine46SceneEditor.h"
#include "Engine46ActorEditor.h"

#include "ui_Engine46MainEditor.h"

class Engine46MainEditor : public QMainWindow {
    Q_OBJECT
public:
    Engine46MainEditor(QWidget* parent = Q_NULLPTR);
    ~Engine46MainEditor();

    void Initialize();

    QWidget* GetSceneRenderWidget() const { return pEngine46SceneEditor->GetRenderWidget(); }

private:
    Ui::Engine46MainEditor ui;

    Engine46SceneEditor* pEngine46SceneEditor;
    Engine46ActorEditor* pEngine46ActorEditor;

    QTreeView* pFileTreeView;

public slots:
    void UpdateFileTreeView();
};
