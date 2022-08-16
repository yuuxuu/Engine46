/**
 * @file Engine46MainEditor.cpp
 * @brief
 * @author 木村優
 * @date 2021/06/26
 */

#include "Engine46Editor.h"

#include "Engine46/CRendererSystem.h"
#include "Engine46/CScene.h"

 // コンストラクタ
Engine46Editor::Engine46Editor(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    resize(QSize(1920, 1080));

    move(QPoint(0, 0));

    pRenderWidget = std::make_unique<MyRenderWidget>(this);
    ui.tabWidget->addTab(pRenderWidget.get(), "RenderWidget");

    pSceneDockWidget = std::make_unique<SceneDockWidget>(this);
    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, pSceneDockWidget.get());

    pInspectorDockWidget = std::make_unique<InspectorDockWidget>(this);
    addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, pInspectorDockWidget.get());

    pFileDockWidget = std::make_unique<FileDockWidget>(this);
    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, pFileDockWidget.get());

    // 接続
    connect(pRenderWidget.get(), &MyRenderWidget::MouseLeftPress, this, &Engine46Editor::SelectActorForMouseLeftPress);

    connect(pInspectorDockWidget->ui.lineEdit_ActorName, &QLineEdit::textChanged, this, &Engine46Editor::UpdateActorName);
}

// デストラクタ
Engine46Editor::~Engine46Editor()
{}

// エディタの初期化
void Engine46Editor::InitializeEditor() {
    pSceneDockWidget->Initialize();
    pInspectorDockWidget->Initialize();
    pFileDockWidget->Initialize();
}

// アクター選択を設定
void Engine46Editor::SetSelectActor(Engine46::CActorBase* pActor) {
    pInspectorDockWidget->SelectActor(pActor);
}

// アクター名を更新
void Engine46Editor::UpdateActorName(const QString& name) {
    
    pInspectorDockWidget->ui.lineEdit_ActorName->blockSignals(true);

    pInspectorDockWidget->UpdateActorName(name);
    pSceneDockWidget->UpdateItemText(name);

    pInspectorDockWidget->ui.lineEdit_ActorName->blockSignals(false);
}


// slots

// 左マウス押下でアクターを選択
void Engine46Editor::SelectActorForMouseLeftPress(const QPoint& point) {
    Engine46::CSceneBase* pScene = Engine46::CRendererSystem::GetRendererSystem().GetRenderScene();
    if (!pScene) return;

    Engine46::VECTOR2 screenSize(static_cast<float>(pRenderWidget->width()), static_cast<float>(pRenderWidget->height()));
    Engine46::VECTOR2 mousePos(static_cast<float>(point.x()), static_cast<float>(point.y()));

    Engine46::CActorBase* pActor = pScene->GetMouseSelectActorFromScene(screenSize, mousePos);
    if (!pActor) return;

    pInspectorDockWidget->SelectActor(pActor);
    pSceneDockWidget->SelectItem(pActor);
}
