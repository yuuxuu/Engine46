/**
 * @file Engine46MainEditor.cpp
 * @brief
 * @author 木村優
 * @date 2021/06/26
 */

#include "Engine46MainEditor.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileSystemModel>

#include "../Engine46/CFileSystem.h"

// コンストラクタ
Engine46MainEditor::Engine46MainEditor(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // シーンエディタ
    pEngine46SceneEditor = new Engine46SceneEditor(this);
    // アクターエディタ
    pEngine46ActorEditor = new Engine46ActorEditor(this);

    QHBoxLayout* pHorizonalLayout = new QHBoxLayout;
    pHorizonalLayout->addWidget(pEngine46SceneEditor);
    pHorizonalLayout->addWidget(pEngine46ActorEditor);

    // ファイルツリービュー
    pFileTreeView = new QTreeView;
    pFileTreeView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pFileTreeView->setMaximumHeight(200);

    QVBoxLayout* pVerticalLayout = new QVBoxLayout(ui.centralwidget);
    pVerticalLayout->addLayout(pHorizonalLayout);
    pVerticalLayout->addWidget(pFileTreeView);
}

// デストラクタ
Engine46MainEditor::~Engine46MainEditor()
{}

// 初期化
void Engine46MainEditor::Initialize() {
    emit this->UpdateFileTreeView();

    emit pEngine46SceneEditor->UpdateSceneTreeView();
}

// ファイルツリービューの更新
void Engine46MainEditor::UpdateFileTreeView() {
    QFileSystemModel* pFileModel = new QFileSystemModel;

    QModelIndex rootIndex = pFileModel->setRootPath(Engine46::RESOURCE_ROOT_PATH);
    pFileTreeView->setModel(pFileModel);

    pFileTreeView->setRootIndex(rootIndex);
}