/**
 * @file Engine46ActorEditor.h
 * @brief
 * @author 木村優
 * @date 2021/06/26
 */

#include "Engine46ActorEditor.h"

#include <Qlabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem> 
#include <QdoubleSpinBox>

 // コンストラクタ
Engine46ActorEditor::Engine46ActorEditor(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    this->setMaximumWidth(300);
}

// デストラクタ
Engine46ActorEditor::~Engine46ActorEditor()
{}

