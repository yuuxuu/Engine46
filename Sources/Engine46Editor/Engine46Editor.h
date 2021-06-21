#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTreeView>

#include "ui_Engine46Editor.h"

class Engine46Editor : public QMainWindow
{
    Q_OBJECT
public:
    Engine46Editor(QWidget *parent = Q_NULLPTR);
    ~Engine46Editor();

    QWidget* GetRenderWidget() const { return pRenderWidget; }

private:
    QWidget* pRenderWidget;

    QTreeView* pSceneTreeView;
    QTreeView* pFileTreeView;

    Ui::Engine46EditorClass ui;

public slots:
    void UpdateSceneTreeView();
    void UpdateFileTreeView();

};
