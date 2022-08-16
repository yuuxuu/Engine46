/**
 * @file InspectorDockWidget.cpp
 * @brief
 * @author 木村優
 * @date 2021/11/07
 */

#include "InspectorDockWidget.h"

#include <QGroupBox>

// コンストラクタ
InspectorDockWidget::InspectorDockWidget(QWidget* parent)
    : QDockWidget(parent),
    pSelectActor(nullptr)
{
    ui.setupUi(this);

    pTransformWidget = std::make_unique<TransformWidget>(this);
    ui.verticalLayout->insertWidget(ui.verticalLayout->count() - 1, pTransformWidget.get());

    pMeshWidget = std::make_unique<MeshWidget>(this);
    ui.verticalLayout->insertWidget(ui.verticalLayout->count() - 1, pMeshWidget.get());

    // 接続
    connect(ui.checkBox_ActorVisible, &QCheckBox::clicked, this, &InspectorDockWidget::UpdateActorVisible);

    connect(pTransformWidget.get(), &TransformWidget::UpdateTransformToActor, this, &InspectorDockWidget::UpdateActorTransform);

    connect(pMeshWidget.get(), &MeshWidget::SetModelMeshToActor, this, &InspectorDockWidget::SetModelMesh);
}

// デストラクタ
InspectorDockWidget::~InspectorDockWidget()
{}

// 初期化
void InspectorDockWidget::Initialize() {

}

// 選択アクターを設定
void InspectorDockWidget::SelectActor(Engine46::CActorBase* pActor) {
    if (!pActor && pSelectActor == pActor) return;

    pSelectActor = pActor;

    ui.lineEdit_ActorName->blockSignals(true);
    ui.lineEdit_ActorName->setText(QString::fromStdString(pSelectActor->GetActorName()));
    ui.lineEdit_ActorName->blockSignals(false);

    ui.checkBox_ActorVisible->blockSignals(true);
    ui.checkBox_ActorVisible->setChecked(pSelectActor->GetVisible());
    ui.checkBox_ActorVisible->blockSignals(false);

    pTransformWidget->SetTransform(pSelectActor->GetTransform());
}

// アクターの名前を更新
void InspectorDockWidget::UpdateActorName(const QString& name) {
    if (!pSelectActor) return;

    pSelectActor->SetActorName(name.toLocal8Bit().constData());
}


// slots

// アクターの表示を更新
void InspectorDockWidget::UpdateActorVisible(const bool visible) {
    if (!pSelectActor) return;

    pSelectActor->SetVisible(visible);
}

// アクターのトランスフォームを更新
void InspectorDockWidget::UpdateActorTransform(const Engine46::Transform& transform) {
    if (!pSelectActor) return;

    pSelectActor->SetTransform(transform);
}

// アクターのモデルメッシュを設定
void InspectorDockWidget::SetModelMesh(const QString& fileName) {
    if (!pSelectActor) return;

    pSelectActor->SetModelMesh(fileName.toLocal8Bit().constData());
}
