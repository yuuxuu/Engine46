/**
 * @file TransformWidget.cpp
 * @brief
 * @author 木村優
 * @date 2022/03/24
 */

#include "TransformWidget.h"

#include <QStandardItem>
#include <QDoubleSpinBox>

#include "Engine46/CScene.h"
#include "Engine46/CRendererSystem.h"

 // コンストラクタ
TransformWidget::TransformWidget(QWidget* parent)
    : QWidget(parent)
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
    connect(ui.doubleSpinBox_PosX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TransformWidget::UpdatePosition);
    connect(ui.doubleSpinBox_PosY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TransformWidget::UpdatePosition);
    connect(ui.doubleSpinBox_PosZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TransformWidget::UpdatePosition);

    connect(ui.doubleSpinBox_RotX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TransformWidget::UpdateRotation);
    connect(ui.doubleSpinBox_RotY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TransformWidget::UpdateRotation);
    connect(ui.doubleSpinBox_RotZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TransformWidget::UpdateRotation);

    connect(ui.doubleSpinBox_ScaX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TransformWidget::UpdateScale);
    connect(ui.doubleSpinBox_ScaY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TransformWidget::UpdateScale);
    connect(ui.doubleSpinBox_ScaZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TransformWidget::UpdateScale);
}

// デストラクタ
TransformWidget::~TransformWidget()
{}

// 更新
void TransformWidget::SetTransform(const Engine46::Transform& transform) {

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

    m_transform = transform;

    ui.doubleSpinBox_PosX->setValue(static_cast<float>(m_transform.pos.x));
    ui.doubleSpinBox_PosY->setValue(static_cast<float>(m_transform.pos.y));
    ui.doubleSpinBox_PosZ->setValue(static_cast<float>(m_transform.pos.z));

    ui.doubleSpinBox_ScaX->setValue(static_cast<float>(m_transform.scale.x));
    ui.doubleSpinBox_ScaY->setValue(static_cast<float>(m_transform.scale.y));
    ui.doubleSpinBox_ScaZ->setValue(static_cast<float>(m_transform.scale.z));

    ui.doubleSpinBox_RotX->setValue(static_cast<float>(m_transform.rotate.x));
    ui.doubleSpinBox_RotY->setValue(static_cast<float>(m_transform.rotate.y));
    ui.doubleSpinBox_RotZ->setValue(static_cast<float>(m_transform.rotate.z));

    for (auto spinBox : pSpinBoxs) {
        spinBox->blockSignals(false);
    }
}


// slots 

// 位置を更新
void TransformWidget::UpdatePosition(double value) {
    Engine46::VECTOR3 pos = m_transform.pos;

    if (sender() == ui.doubleSpinBox_PosX) {
        pos.x = value;
    }
    else if (sender() == ui.doubleSpinBox_PosY) {
        pos.y = value;
    }
    else if (sender() == ui.doubleSpinBox_PosZ) {
        pos.z = value;
    }

    m_transform.pos = pos;

    emit UpdateTransformToActor(m_transform);
}

// 回転を更新
void TransformWidget::UpdateRotation(double value) {
    Engine46::VECTOR3 rotate = m_transform.rotate;

    if (sender() == ui.doubleSpinBox_RotX) {
        rotate.x = Engine46::DegreeToRadian(value);
    }
    else if (sender() == ui.doubleSpinBox_RotY) {
        rotate.y = Engine46::DegreeToRadian(value);
    }
    else if (sender() == ui.doubleSpinBox_RotZ) {
        rotate.z = Engine46::DegreeToRadian(value);
    }

    m_transform.rotate = rotate;

    emit UpdateTransformToActor(m_transform);
}

// 拡縮を更新
void TransformWidget::UpdateScale(double value) {
    Engine46::VECTOR3 scale = m_transform.scale;

    if (sender() == ui.doubleSpinBox_ScaX) {
        scale.x = value;
    }
    else if (sender() == ui.doubleSpinBox_ScaY) {
        scale.y = value;
    }
    else if (sender() == ui.doubleSpinBox_ScaZ) {
        scale.z = value;
    }

    m_transform.scale = scale;

    emit UpdateTransformToActor(m_transform);
}
