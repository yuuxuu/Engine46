/**
 * @file QMyTreeView.h
 * @brief
 * @author 木村優
 * @date 2021/07/04
 */

#pragma once

#include <QTreeView>

class QMyTreeView : public QTreeView {
public:
    QMyTreeView(QWidget* parent = Q_NULLPTR);
    ~QMyTreeView();

private:
    void keyPressEvent(QKeyEvent* event) override;
};

