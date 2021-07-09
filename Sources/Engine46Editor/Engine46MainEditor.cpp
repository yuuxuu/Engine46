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
    // ファイルエディタ
    pEngine46FileEditor = new Engine46FileEditor(this);

    // レンダーウェジット
    pRenderWidget = new QMyRenderWidget(ui.centralwidget);
    pRenderWidget->resize(QSize(1280, 720));

    QHBoxLayout* pHorizonalLayout = new QHBoxLayout;
    pHorizonalLayout->addWidget(pEngine46SceneEditor);
    pHorizonalLayout->addWidget(pRenderWidget);
    pHorizonalLayout->addWidget(pEngine46ActorEditor);

    QVBoxLayout* pVerticalLayout = new QVBoxLayout(ui.centralwidget);
    pVerticalLayout->addLayout(pHorizonalLayout);
    pVerticalLayout->addWidget(pEngine46FileEditor);

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
    pEngine46FileEditor->UpdateFileTreeView();

    pEngine46SceneEditor->UpdateSceneTreeView();
}

// アクター名変更を各エディタへ知らせる
void Engine46MainEditor::ChangeValueActorName() {
    QString string = pEngine46ActorEditor->ui.lineEdit_ActorName->text();

    pEngine46SceneEditor->ChangeValueReflectToName(string);
    pEngine46ActorEditor->ChangeValueReflectToName(string);
}