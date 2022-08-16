/**
 * @file MeshWidget.h
 * @brief
 * @author 木村優
 * @date 2022/03/24
 */

#include "MeshWidget.h"

#include <QFileDialog>
#include <QFileInfo>

#include "Engine46/CFileManager.h"

#include "Engine46/CModelMesh.h"

// コンストラクタ
MeshWidget::MeshWidget(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    // 接続
    connect(ui.pushButton_LoadModel, &QPushButton::clicked, this, &MeshWidget::OpenFile);
}

// デストラクタ
MeshWidget::~MeshWidget()
{}


// slots

void MeshWidget::OpenFile() {

    QString fileFlter = QString::fromLocal8Bit("(*.fbx *.obj)");
    QString dir = QString::fromUtf8(Engine46::CFileManager::ResourceRootPath().c_str());

    QString fileName = 
        QFileDialog::getOpenFileName(
        this,
        QString::fromLocal8Bit("ファイルを開く"),
        dir,
        fileFlter,
        nullptr,
        QFileDialog::DontUseCustomDirectoryIcons);

    if (fileName.isEmpty())  return;

    QFileInfo info(fileName);

    ui.lineEdit->setText(info.fileName());

    emit SetModelMeshToActor(info.fileName());
}
