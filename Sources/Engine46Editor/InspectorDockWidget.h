/**
 * @file InspectorDockWidget.h
 * @brief
 * @author 木村優
 * @date 2022/03/23
 */

#pragma once

#include <qdockwidget.h>

#include "TransformWidget.h"
#include "MeshWidget.h"

#include "ui_InspectorDockWidget.h"

#include "Engine46/CActor.h"

class InspectorDockWidget : public QDockWidget {
public:
    Ui::InspectorDockWidget ui;

private:
    std::unique_ptr<TransformWidget> pTransformWidget;
    std::unique_ptr<MeshWidget> pMeshWidget;

    Engine46::CActorBase* pSelectActor;

public:
    InspectorDockWidget(QWidget* parent = nullptr);
    ~InspectorDockWidget();

    void Initialize();

    void SelectActor(Engine46::CActorBase* pActor);

    void UpdateActorName(const QString& name);

public slots:
    void UpdateActorVisible(const bool visible);
    void UpdateActorTransform(const Engine46::Transform& transform);

    void SetModelMesh(const QString& fileName);
};

