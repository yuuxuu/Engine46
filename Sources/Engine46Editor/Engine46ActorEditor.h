/**
 * @file Engine46ActorEditor.h
 * @brief
 * @author 木村優
 * @date 2021/06/26
 */

#pragma once

#include <QWidget>

#include "ui_Engine46ActorEditor.h"

class Engine46ActorEditor : public QWidget {
	Q_OBJECT
public:
	Engine46ActorEditor(QWidget* parent = Q_NULLPTR);
	~Engine46ActorEditor();

public:
	Ui::Engine46ActorEditor ui;

public slots:

};

