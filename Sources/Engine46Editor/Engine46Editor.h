#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Engine46Editor.h"

class Engine46Editor : public QMainWindow
{
    Q_OBJECT
public:
    Engine46Editor(QWidget *parent = Q_NULLPTR);

    QWidget* GetRenderWidget() const { return ui.RenderWidget; }
private:
    Ui::Engine46EditorClass ui;
};
