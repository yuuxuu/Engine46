/**
 * @file Engine46ActorEditor.h
 * @brief
 * @author 木村優
 * @date 2021/06/26
 */

#include "Engine46ActorEditor.h"

#include <QStandardItem>
#include <QSpinBox>

#include "../Engine46/CActor.h"
#include "../Engine46/CScene.h"
#include "../Engine46/CRendererSystem.h"

 // コンストラクタ
Engine46ActorEditor::Engine46ActorEditor(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    this->setMaximumWidth(300);

    QDoubleSpinBox* pSpinBoxs[] = {
        ui.doubleSpinBox_PosX,
        ui.doubleSpinBox_PosY,
        ui.doubleSpinBox_PosZ,
        ui.doubleSpinBox_RotX,
        ui.doubleSpinBox_RotY,
        ui.doubleSpinBox_RotZ,
        ui.doubleSpinBox_ScaX,
        ui.doubleSpinBox_ScaY,
        ui.doubleSpinBox_ScaZ,
    };

    for (auto spinBox : pSpinBoxs) {
        spinBox->setRange(-1000, 1000);
    }

    // 接続
    connect(ui.doubleSpinBox_PosX, SIGNAL(valueChanged(double)), this, SLOT(ChangeValueReflectToPos(double)));
    connect(ui.doubleSpinBox_PosY, SIGNAL(valueChanged(double)), this, SLOT(ChangeValueReflectToPos(double)));
    connect(ui.doubleSpinBox_PosZ, SIGNAL(valueChanged(double)), this, SLOT(ChangeValueReflectToPos(double)));

    connect(ui.doubleSpinBox_RotX, SIGNAL(valueChanged(double)), this, SLOT(ChangeValueReflectToRotation(double)));
    connect(ui.doubleSpinBox_RotY, SIGNAL(valueChanged(double)), this, SLOT(ChangeValueReflectToRotation(double)));
    connect(ui.doubleSpinBox_RotZ, SIGNAL(valueChanged(double)), this, SLOT(ChangeValueReflectToRotation(double)));

    connect(ui.doubleSpinBox_ScaX, SIGNAL(valueChanged(double)), this, SLOT(ChangeValueReflectToScale(double)));
    connect(ui.doubleSpinBox_ScaY, SIGNAL(valueChanged(double)), this, SLOT(ChangeValueReflectToScale(double)));
    connect(ui.doubleSpinBox_ScaZ, SIGNAL(valueChanged(double)), this, SLOT(ChangeValueReflectToScale(double)));
}

// デストラクタ
Engine46ActorEditor::~Engine46ActorEditor()
{}

// 選択中アクターをエディターへ反映
void Engine46ActorEditor::SelectActorReflectToEditor() {
    if (pSelectActor) {
        ui.lineEdit_ActorName->setText(pSelectActor->GetActorName().c_str());

        Engine46::Transform transform = pSelectActor->GetTransform();

        ui.doubleSpinBox_PosX->setValue(transform.pos.x);
        ui.doubleSpinBox_PosY->setValue(transform.pos.y);
        ui.doubleSpinBox_PosZ->setValue(transform.pos.z);

        ui.doubleSpinBox_RotX->setValue(transform.rotation.x);
        ui.doubleSpinBox_RotY->setValue(transform.rotation.y);
        ui.doubleSpinBox_RotZ->setValue(transform.rotation.z);

        ui.doubleSpinBox_ScaX->setValue(transform.scale.x);
        ui.doubleSpinBox_ScaY->setValue(transform.scale.y);
        ui.doubleSpinBox_ScaZ->setValue(transform.scale.z);
    }
}

// エディターを選択中アクターへ反映
void Engine46ActorEditor::EditorReflectToSelectActor() {
    if (pSelectActor) {
        ui.lineEdit_ActorName->setText(pSelectActor->GetActorName().c_str());

        Engine46::Transform transform;

        transform.pos.x = (float)ui.doubleSpinBox_PosX->value();
        transform.pos.y = (float)ui.doubleSpinBox_PosY->value();
        transform.pos.z = (float)ui.doubleSpinBox_PosZ->value();

        transform.rotation.x = (float)ui.doubleSpinBox_RotX->value();
        transform.rotation.y = (float)ui.doubleSpinBox_RotY->value();
        transform.rotation.z = (float)ui.doubleSpinBox_RotZ->value();

        transform.scale.x = (float)ui.doubleSpinBox_ScaX->value();
        transform.scale.y = (float)ui.doubleSpinBox_ScaY->value();
        transform.scale.z = (float)ui.doubleSpinBox_ScaZ->value();

        pSelectActor->SetTransform(transform);
    }
}

// 選択しているアクターを設定
void Engine46ActorEditor::SetSelectActor(const QModelIndex& index) {
    std::string actorName = index.data().toString().toLocal8Bit();
    
    if (pSelectActor) {
        if (actorName == pSelectActor->GetActorName()) return;
    }

    Engine46::CSceneBase* pScene = Engine46::CRendererSystem::GetRendererSystem().GetRenderScene();
    if (pScene) {
        pSelectActor = pScene->GetActorFromActorName(actorName);
        this->SelectActorReflectToEditor();
    }
}

// 選択しているアクターへの名前変更を反映
void Engine46ActorEditor::ChangeValueReflectToName(const QString& string) {
    if (pSelectActor) {
        std::string str = string.toLocal8Bit();

        if (str == pSelectActor->GetActorName()) return;

        pSelectActor->SetActorName(str);
    }
}

// 選択しているアクターへの位置を反映
void Engine46ActorEditor::ChangeValueReflectToPos(double value) {
    if (pSelectActor) {
        Engine46::VECTOR3 pos;

        pos.x = (float)ui.doubleSpinBox_PosX->value();
        pos.y = (float)ui.doubleSpinBox_PosY->value();
        pos.z = (float)ui.doubleSpinBox_PosZ->value();

        pSelectActor->SetPos(pos);
    }
}

// 選択しているアクターへの回転を反映
void Engine46ActorEditor::ChangeValueReflectToRotation(double value) {
    if (pSelectActor) {
        Engine46::VECTOR3 rotation;

        rotation.x = (float)ui.doubleSpinBox_RotX->value();
        rotation.y = (float)ui.doubleSpinBox_RotY->value();
        rotation.z = (float)ui.doubleSpinBox_RotZ->value();

        pSelectActor->SetRotation(rotation);
    }
}

// 選択しているアクターへの拡縮を反映
void Engine46ActorEditor::ChangeValueReflectToScale(double value) {
    if (pSelectActor) {
        Engine46::VECTOR3 scale;

        scale.x = (float)ui.doubleSpinBox_ScaX->value();
        scale.y = (float)ui.doubleSpinBox_ScaY->value();
        scale.z = (float)ui.doubleSpinBox_ScaZ->value();

        pSelectActor->SetScale(scale);
    }
}

