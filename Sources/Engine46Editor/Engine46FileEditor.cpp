/**
 * @file Engine46FileEditor.cpp
 * @brief
 * @author 木村優
 * @date 2021/07/08
 */

#include "Engine46FileEditor.h"

#include <QFileSystemModel>

#include "../Engine46/CFileSystem.h"

// コンストラクタ
Engine46FileEditor::Engine46FileEditor(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.treeView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	ui.treeView->setMaximumHeight(200);
	ui.treeView->setMaximumWidth(parent->width() / 3);

	ui.listView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	ui.listView->setMaximumHeight(200);
	ui.listView->setMaximumWidth(parent->width() / 2);

	ui.listView->setViewMode(QListView::IconMode);
	ui.listView->setFlow(QListView::LeftToRight);

	// 接続
	connect(ui.treeView, &QAbstractItemView::clicked, this, &Engine46FileEditor::SelectItem);
	connect(ui.listView, &QAbstractItemView::doubleClicked, this, &Engine46FileEditor::SelectItem);
}

// デストラクタ
Engine46FileEditor::~Engine46FileEditor()
{}

// ファイルエディタの初期化
void Engine46FileEditor::InitializeFileEditor() {
	QFileSystemModel* pFileModel = new QFileSystemModel;

	QModelIndex rootIndex = pFileModel->setRootPath(Engine46::RESOURCE_ROOT_PATH);

	ui.treeView->setModel(pFileModel);
	ui.treeView->setRootIndex(rootIndex);

	ui.listView->setModel(pFileModel);
	ui.listView->setRootIndex(rootIndex);
}

// 選択しているアイテムを設定
void Engine46FileEditor::SelectItem(const QModelIndex& index) {
	ui.listView->setRootIndex(index);
}