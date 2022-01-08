/**
 * @file MyInspectorWidget.cpp
 * @brief
 * @author 木村優
 * @date 2021/06/26
 */

#include "MyInspectorWidget.h"

#include <QStandardItem>
#include <QDoubleSpinBox>

#include "Engine46/CScene.h"
#include "Engine46/CRendererSystem.h"

 // コンストラクタ
MyInspectorWidget::MyInspectorWidget(QWidget* parent)
    : QWidget(parent),
    pSelectActor(nullptr)
{
    ui.setupUi(this);

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
        spinBox->setFixedSize(QSize(100, 30));
    }

    // 接続
    connect(ui.checkBox_visible, &QCheckBox::clicked, this, &MyInspectorWidget::UpdateActorVisible);

    connect(ui.lineEdit_ActorName, &QLineEdit::textChanged, this, &MyInspectorWidget::UpdateActorName);

    connect(ui.doubleSpinBox_PosX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MyInspectorWidget::UpdateActorPosition);
    connect(ui.doubleSpinBox_PosY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MyInspectorWidget::UpdateActorPosition);
    connect(ui.doubleSpinBox_PosZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MyInspectorWidget::UpdateActorPosition);

    connect(ui.doubleSpinBox_RotX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MyInspectorWidget::UpdateActorRotation);
    connect(ui.doubleSpinBox_RotY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MyInspectorWidget::UpdateActorRotation);
    connect(ui.doubleSpinBox_RotZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MyInspectorWidget::UpdateActorRotation);

    connect(ui.doubleSpinBox_ScaX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MyInspectorWidget::UpdateActorScale);
    connect(ui.doubleSpinBox_ScaY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MyInspectorWidget::UpdateActorScale);
    connect(ui.doubleSpinBox_ScaZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MyInspectorWidget::UpdateActorScale);
}

// デストラクタ
MyInspectorWidget::~MyInspectorWidget()
{}

// 選択アクターを設定
void MyInspectorWidget::SetSelectActor(Engine46::CActorBase* pActor) {
    if (pSelectActor == pActor) return;

    pSelectActor = pActor;

    ui.checkBox_visible->setChecked(pActor->GetVisible());

    ui.lineEdit_ActorName->blockSignals(true);

    ui.lineEdit_ActorName->setText(QString(pActor->GetActorName().c_str()));

    ui.lineEdit_ActorName->blockSignals(false);

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
        spinBox->blockSignals(true);
    }

    Engine46::Transform transform = pSelectActor->GetTransform();
    ui.doubleSpinBox_PosX->setValue(static_cast<float>(transform.pos.x));
    ui.doubleSpinBox_PosY->setValue(static_cast<float>(transform.pos.y));
    ui.doubleSpinBox_PosZ->setValue(static_cast<float>(transform.pos.z));

    ui.doubleSpinBox_ScaX->setValue(static_cast<float>(transform.scale.x));
    ui.doubleSpinBox_ScaY->setValue(static_cast<float>(transform.scale.y));
    ui.doubleSpinBox_ScaZ->setValue(static_cast<float>(transform.scale.z));

    ui.doubleSpinBox_RotX->setValue(static_cast<float>(transform.rotation.x));
    ui.doubleSpinBox_RotY->setValue(static_cast<float>(transform.rotation.y));
    ui.doubleSpinBox_RotZ->setValue(static_cast<float>(transform.rotation.z));

    for (auto spinBox : pSpinBoxs) {
        spinBox->blockSignals(false);
    }
}

//////////////////////
// slots 

// アクターの表示を反映
void MyInspectorWidget::UpdateActorVisible(bool isVisible) {
    if (!pSelectActor) return;

    pSelectActor->SetVisible(isVisible);
}

// アクターへ名前を反映
void MyInspectorWidget::UpdateActorName(const QString& name) {
    if (!pSelectActor) return;

    std::string newName = pSelectActor->GetActorName();

    if (name == QString(newName.c_str())) return;

    pSelectActor->SetActorName(newName);
}

// アクターへ位置を反映
void MyInspectorWidget::UpdateActorPosition(double value) {
    if (!pSelectActor) return;

    Engine46::VECTOR3 pos = pSelectActor->GetPos();

    if (sender() == ui.doubleSpinBox_PosX) {
        pos.x = value;
    }
    else if (sender() == ui.doubleSpinBox_PosY) {
        pos.y = value;
    }
    else if (sender() == ui.doubleSpinBox_PosZ) {
        pos.z = value;
    }

    pSelectActor->SetPos(pos);
}

// アクターへ回転を反映
void MyInspectorWidget::UpdateActorRotation(double value) {
    if (!pSelectActor) return;

    Engine46::VECTOR3 rotation = pSelectActor->GetRotation();

    if (sender() == ui.doubleSpinBox_RotX) {
        rotation.x = Engine46::DegreeToRadian(value);
    }
    else if (sender() == ui.doubleSpinBox_RotY) {
        rotation.y = Engine46::DegreeToRadian(value);
    }
    else if (sender() == ui.doubleSpinBox_RotZ) {
        rotation.z = Engine46::DegreeToRadian(value);
    }

    pSelectActor->SetRotation(rotation);
}

// アクターへ拡縮を反映
void MyInspectorWidget::UpdateActorScale(double value) {
    if (!pSelectActor) return;

    Engine46::VECTOR3 scale = pSelectActor->GetScale();

    if (sender() == ui.doubleSpinBox_ScaX) {
        scale.x = value;
    }
    else if (sender() == ui.doubleSpinBox_ScaY) {
        scale.y = value;
    }
    else if (sender() == ui.doubleSpinBox_ScaZ) {
        scale.z = value;
    }

    pSelectActor->SetScale(scale);
}
