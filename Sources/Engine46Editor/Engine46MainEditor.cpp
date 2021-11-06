/**
 * @file Engine46MainEditor.cpp
 * @brief
 * @author 木村優
 * @date 2021/06/26
 */

#include "Engine46MainEditor.h"

#include <QDockWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileSystemModel>

#include "Engine46/CFileSystem.h"
#include "Engine46/CRendererSystem.h"
#include "Engine46/CScene.h"

 // コンストラクタ
Engine46MainEditor::Engine46MainEditor(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    this->resize(QSize(1920, 1080));

    this->setStyleSheet(
        "QWidget {"
        "border: 2px solid grey;"
        "}"
    );

    // シーンエディタ
    pEngine46SceneEditor = new Engine46SceneEditor(this);
    // アクターエディタ
    pEngine46ActorEditor = new Engine46ActorEditor(this);
    // ファイルエディタ
    pEngine46FileEditor = new Engine46FileEditor(this);

    ui.dockWidget_Scene->setWidget(pEngine46SceneEditor);
    ui.dockWidget_Actor->setWidget(pEngine46ActorEditor);
    ui.dockWidget_File->setWidget(pEngine46FileEditor);

    // レンダーウェジット
    pRenderWidget = new QMyRenderWidget(this);

    ui.tabWidget->addTab(pRenderWidget, "RenderWidget");

    // 接続
    connect(pRenderWidget, &QMyRenderWidget::MouseLeftPress, this, &Engine46MainEditor::SelectActorForMouseLeftPress);

    connect(pEngine46SceneEditor->ui.sceneTreeView, &QAbstractItemView::clicked, pEngine46ActorEditor, &Engine46ActorEditor::SetSelectActor);
    connect(pEngine46SceneEditor->ui.sceneTreeView, &QAbstractItemView::clicked, pEngine46SceneEditor, &Engine46SceneEditor::SelectItem);

    connect(pEngine46ActorEditor->ui.lineEdit_ActorName, &QLineEdit::returnPressed, this, &Engine46MainEditor::ChangeValueActorName);
}

// デストラクタ
Engine46MainEditor::~Engine46MainEditor()
{}

// メインエディタの初期化
void Engine46MainEditor::InitializeMainEditor() {
    pEngine46SceneEditor->InitializeSceneEditor();

    pEngine46FileEditor->InitializeFileEditor();
}

///////////
// slots 
//////////

// アクター名変更を各エディタへ知らせる
void Engine46MainEditor::ChangeValueActorName() {
    QString string = pEngine46ActorEditor->ui.lineEdit_ActorName->text();

    pEngine46SceneEditor->ChangeValueReflectToName(string);
    pEngine46ActorEditor->ChangeValueReflectToName(string);
}

// 左マウス押下でアクターを選択
void Engine46MainEditor::SelectActorForMouseLeftPress(const QPoint& point) {
    Engine46::CSceneBase* pScene = Engine46::CRendererSystem::GetRendererSystem().GetRenderScene();
    if (pScene) {
        Engine46::VECTOR2 screenSize(static_cast<float>(pRenderWidget->width()), static_cast<float>(pRenderWidget->height()));
        Engine46::VECTOR2 mousePos(static_cast<float>(point.x()), static_cast<float>(point.y()));
        
        Engine46::CActorBase* pActor = pScene->GetMouseSelectActorFromScene(screenSize, mousePos);
        if (pActor) {

        }
    }
}