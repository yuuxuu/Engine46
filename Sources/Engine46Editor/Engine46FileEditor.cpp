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

	ui.listView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	ui.listView->setMaximumHeight(200);
}

// デストラクタ
Engine46FileEditor::~Engine46FileEditor()
{}

// ファイルツリービューの更新
void Engine46FileEditor::UpdateFileTreeView(){
	QFileSystemModel* pFileModel = new QFileSystemModel;

	QModelIndex rootIndex = pFileModel->setRootPath(Engine46::RESOURCE_ROOT_PATH);
	ui.treeView->setModel(pFileModel);

	ui.treeView->setRootIndex(rootIndex);
}