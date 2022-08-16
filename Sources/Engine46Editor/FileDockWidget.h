/**
 * @file FileDockWidget.h
 * @brief
 * @author 木村優
 * @date 2022/03/23
 */

#pragma once

#include <qdockwidget.h>

#include "ui_FileDockWidget .h"

class FileDockWidget : public QDockWidget {
public:
    Ui::FileDockWidget ui;

public:
    FileDockWidget(QWidget* parent = nullptr);
    ~FileDockWidget();

    void Initialize();
};

