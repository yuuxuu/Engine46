/**
 * @file Engine46FileEditor.h
 * @brief
 * @author 木村優
 * @date 2021/07/08
 */

#pragma once

#include <QWidget>

#include "ui_Engine46FileEditor.h"

class Engine46FileEditor : public QWidget {
public:
	Engine46FileEditor(QWidget* parent = Q_NULLPTR);
	~Engine46FileEditor();

public:
	Ui::Engine46FileEditor ui;

public slots:
	void UpdateFileTreeView();
};

