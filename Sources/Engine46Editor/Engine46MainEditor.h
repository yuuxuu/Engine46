/**
 * @file Engine46MainEditor.h
 * @brief
 * @author 木村優
 * @date 2021/06/26
 */

#pragma once

#include <QMainWindow>
#include <QTreeView>

#include "QMyRenderWidget.h"

#include "Engine46SceneEditor.h"
#include "Engine46ActorEditor.h"
#include "Engine46FileEditor.h"

#include "ui_Engine46MainEditor.h"

class Engine46MainEditor : public QMainWindow {
public:
    Engine46MainEditor(QWidget* parent = Q_NULLPTR);
    ~Engine46MainEditor();

    void InitializeMainEditor();

    QWidget* GetRenderWidget() const { return pRenderWidget; }

private:
    Ui::Engine46MainEditor ui;

    QMyRenderWidget* pRenderWidget;

    Engine46SceneEditor* pEngine46SceneEditor;
    Engine46ActorEditor* pEngine46ActorEditor;
    Engine46FileEditor* pEngine46FileEditor;

public slots:
    void ChangeValueActorName();
};
