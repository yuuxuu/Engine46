/**
 * @file QMyRenderWidget.h
 * @brief
 * @author 木村優
 * @date 2021/07/02
 */

#pragma once

#include <QWidget>

class QMyRenderWidget : public QWidget
{
	Q_OBJECT
public:
	QMyRenderWidget(QWidget* parent = Q_NULLPTR);
	~QMyRenderWidget();

private:

	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;
};

