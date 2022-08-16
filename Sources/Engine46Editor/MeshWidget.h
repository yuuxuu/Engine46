/**
 * @file MeshWidget.h
 * @brief
 * @author 木村優
 * @date 2022/03/24
 */

#pragma once

#include <QWidget>

#include "ui_MeshWidget.h"

class CMeshBase;
class CModelMesh;

class MeshWidget : public QWidget {
    Q_OBJECT
private:
    Ui::MeshWidget ui;

    CModelMesh* m_pModelMesh;

public:
    MeshWidget(QWidget* parent = nullptr);
    ~MeshWidget();

public slots:
    void OpenFile();

Q_SIGNALS:
    void SetModelMeshToActor(const QString& fileName);

};
