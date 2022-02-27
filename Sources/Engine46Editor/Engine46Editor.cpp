/**
 * @file Engine46MainEditor.cpp
 * @brief
 * @author 木村優
 * @date 2021/06/26
 */

#include "Engine46Editor.h"

#include <QStandardItemModel>
#include <QFileSystemModel>

#include "MyStandardItem.h"

#include "Engine46/CFileManager.h"
#include "Engine46/CRendererSystem.h"
#include "Engine46/CScene.h"

 // コンストラクタ
Engine46Editor::Engine46Editor(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    resize(QSize(1920, 1080));

    move(QPoint(0, 0));

    // レンダーウェジット
    pRenderWidget = std::make_unique<MyRenderWidget>(this);
    ui.tabWidget->addTab(pRenderWidget.get(), "RenderWidget");

    // インスペクターウェジット
    pInspectorWidget = std::make_unique<MyInspectorWidget>(this);
    ui.dockWidget_Inspector->setWidget(pInspectorWidget.get());

    ui.FileListView->setViewMode(QListView::IconMode);
    ui.FileListView->setFlow(QListView::LeftToRight);

    // 接続
    //connect(ui.checkBox_visible, &QCheckBox::clicked, this, )

    connect(ui.sceneTreeView, &QAbstractItemView::clicked, this, &Engine46Editor::SelectActorForSceneTreeView);

    connect(pRenderWidget.get(), &MyRenderWidget::MouseLeftPress, this, &Engine46Editor::SelectActorForMouseLeftPress);
}

// デストラクタ
Engine46Editor::~Engine46Editor()
{}

// エディタの初期化
void Engine46Editor::InitializeEditor() {

    InitializeSceneTreeView();
    InitializeFileTreeView();
}

// シーンツリービュー用アクター再帰検索
static void RecursiveActor(QStandardItem* pRootItem, Engine46::CActorBase* pRootActor) {
    int rowIndex = 0;
    for (auto& pChildActor : pRootActor->GetChildActorList()) {
        
        if (!pChildActor) continue;

        MyStandardItem* pChildItem = new MyStandardItem(pChildActor);

        pRootItem->setChild(rowIndex++, 0, pChildItem);

        RecursiveActor(pChildItem, pChildActor);
    }
}

// シーンツリービュー初期化
void Engine46Editor::InitializeSceneTreeView() {
    Engine46::CSceneBase* pScene = Engine46::CRendererSystem::GetRendererSystem().GetRenderScene();
    if (!pScene) return;

    Engine46::CActorBase* pRootActor = pScene->GetRootActor();
    if (!pRootActor) return;

    ui.sceneTreeView->reset();

    QStandardItemModel* pItemModel = new QStandardItemModel;
    QStringList strList = {
        QString(pScene->GetSceneName().c_str()),
    };
    pItemModel->setHorizontalHeaderLabels(strList);

    MyStandardItem* pRootItem = new MyStandardItem(pRootActor);
    
    RecursiveActor(pRootItem, pRootActor);
    pItemModel->appendRow(pRootItem);

    ui.sceneTreeView->setModel(pItemModel);

    ui.sceneTreeView->expandAll();
}

// ファイルツリービュー初期化
void Engine46Editor::InitializeFileTreeView() {
    QFileSystemModel* pFileModel = new QFileSystemModel;

    // ルートパスを設定
    QModelIndex rootIndex = pFileModel->setRootPath(Engine46::CFileManager::ResourceRootPath().c_str());

    ui.FileTreeView->setModel(pFileModel);
    ui.FileTreeView->setRootIndex(rootIndex);

    ui.FileListView->setModel(pFileModel);
    ui.FileListView->setRootIndex(rootIndex);
}

//////////////////////
// slots 

// 左マウス押下でアクターを選択
void Engine46Editor::SelectActorForMouseLeftPress(const QPoint& point) {
    Engine46::CSceneBase* pScene = Engine46::CRendererSystem::GetRendererSystem().GetRenderScene();
    if (!pScene) return;

    Engine46::VECTOR2 screenSize(static_cast<float>(pRenderWidget->width()), static_cast<float>(pRenderWidget->height()));
    Engine46::VECTOR2 mousePos(static_cast<float>(point.x()), static_cast<float>(point.y()));

    Engine46::CActorBase* pActor = pScene->GetMouseSelectActorFromScene(screenSize, mousePos);
    if (!pActor) return;

    pInspectorWidget->SetSelectActor(pActor);
}

// シーンツリービューからアクターを選択
void Engine46Editor::SelectActorForSceneTreeView(const QModelIndex& index) {
    QStandardItemModel* pItemModel = dynamic_cast<QStandardItemModel*>(ui.sceneTreeView->model());
    if (!pItemModel) return;

    MyStandardItem* pItem = dynamic_cast<MyStandardItem*>(pItemModel->itemFromIndex(index));
    if (!pItem) return;

    Engine46::CActorBase* pActor = pItem->GetActor();
    if (!pActor) return;

    pInspectorWidget->SetSelectActor(pActor);
}
