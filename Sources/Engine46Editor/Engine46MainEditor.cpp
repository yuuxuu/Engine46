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

    // 接続
    connect(pEngine46SceneEditor->ui.sceneTreeView, &QAbstractItemView::clicked, pEngine46ActorEditor, &Engine46ActorEditor::SetSelectActor);
    connect(pEngine46SceneEditor->ui.sceneTreeView, &QAbstractItemView::clicked, pEngine46SceneEditor, &Engine46SceneEditor::SetSelectItem);

    connect(pEngine46ActorEditor->ui.lineEdit_ActorName, &QLineEdit::returnPressed, this, &Engine46MainEditor::ChangeValueActorName);
}

// デストラクタ
Engine46MainEditor::~Engine46MainEditor()
{}

// 初期化
void Engine46MainEditor::Initialize() {
    UpdateFileTreeView();

    pEngine46SceneEditor->UpdateSceneTreeView();
}

// アクター名変更を各エディタへ知らせる
void Engine46MainEditor::ChangeValueActorName() {
    QString string = pEngine46ActorEditor->ui.lineEdit_ActorName->text();

    pEngine46SceneEditor->ChangeValueReflectToName(string);
    pEngine46ActorEditor->ChangeValueReflectToName(string);
}

// ファイルツリービューの更新
void Engine46MainEditor::UpdateFileTreeView() {
    QFileSystemModel* pFileModel = new QFileSystemModel;

    QModelIndex rootIndex = pFileModel->setRootPath(Engine46::RESOURCE_ROOT_PATH);
    pFileTreeView->setModel(pFileModel);

    pFileTreeView->setRootIndex(rootIndex);
}