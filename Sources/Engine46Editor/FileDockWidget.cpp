/**
 * @file FileDockWidget.cpp
 * @brief
 * @author 木村優
 * @date 2022/03/23
 */

#include "FileDockWidget.h"

#include <QFileSystemModel>

#include "Engine46/CFileManager.h"

// コンストラクタ
FileDockWidget::FileDockWidget(QWidget* parent)
    : QDockWidget(parent)
{
    ui.setupUi(this);

    ui.listView->setViewMode(QListView::IconMode);
    ui.listView->setFlow(QListView::LeftToRight);
}

// デストラクタ
FileDockWidget::~FileDockWidget()
{}

// 初期化
void FileDockWidget::Initialize() {
    QFileSystemModel* pFileModel = new QFileSystemModel;

    // ルートパスを設定
    QModelIndex rootIndex = pFileModel->setRootPath(Engine46::CFileManager::ResourceRootPath().c_str());

    ui.listView->setModel(pFileModel);
    ui.listView->setRootIndex(rootIndex);
}
