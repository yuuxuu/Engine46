/**
 * @file SceneDockWidget.cpp
 * @brief
 * @author 木村優
 * @date 2022/03/23
 */

#include "SceneDockWidget.h"

#include <QStandardItemModel>
#include <QMenu>

#include "Engine46Editor.h"
#include "MyStandardItem.h"

#include "Engine46/CGameSystem.h"
#include "Engine46/CRendererSystem.h"
#include "Engine46/CActorManager.h"
#include "Engine46/CScene.h"

// コンストラクタ
SceneDockWidget::SceneDockWidget(QWidget* parent)
    : QDockWidget(parent),
    pSelectItem(nullptr)
{
    ui.setupUi(this);

    ui.treeView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

    // 接続
    connect(ui.treeView, &QWidget::customContextMenuRequested, this, &SceneDockWidget::OnContextMenu);
    connect(ui.treeView, &QAbstractItemView::clicked, this, &SceneDockWidget::SelectActor);
}

// デストラクタ
SceneDockWidget::~SceneDockWidget()
{}

// ツリービュー用アクター再帰検索
static void RecursiveActor(QStandardItem* pRootItem, Engine46::CActorBase* pRootActor) {
    int rowIndex = 0;
    for (auto pChildActor : pRootActor->GetChildActorList()) {

        if (!pChildActor) continue;

        MyStandardItem* pChildItem = new MyStandardItem(pChildActor);

        pRootItem->setChild(rowIndex++, 0, pChildItem);

        RecursiveActor(pChildItem, pChildActor);
    }
}

// 初期化
void SceneDockWidget::Initialize() {
    Engine46::CSceneBase* pScene = Engine46::CRendererSystem::GetRendererSystem().GetRenderScene();
    if (!pScene) return;

    Engine46::CActorBase* pRootActor = pScene->GetRootActor();
    if (!pRootActor) return;

    ui.treeView->reset();

    QStandardItemModel* pItemModel = new QStandardItemModel;
    QStringList strList = {
        QString::fromStdString(pScene->GetSceneName()),
    };
    pItemModel->setHorizontalHeaderLabels(strList);

    MyStandardItem* pRootItem = new MyStandardItem(pRootActor);

    RecursiveActor(pRootItem, pRootActor);
    pItemModel->appendRow(pRootItem);

    ui.treeView->setModel(pItemModel);

    ui.treeView->expandAll();
}

// ツリーへアイテムを追加
void SceneDockWidget::AddItemToTreeView(Engine46::CActorBase* pActor) {
    QStandardItemModel* pItemModel = dynamic_cast<QStandardItemModel*>(ui.treeView->model());
    if (!pItemModel) return;

    QStandardItem* rootItem = pItemModel->item(0);
    if (!rootItem) return;

    MyStandardItem* pChildItem = new MyStandardItem(pActor);
    rootItem->setChild(rootItem->rowCount(), 0, pChildItem);
}

void SceneDockWidget::UpdateItemText(const QString& text) {
    if (!pSelectItem) return;

    pSelectItem->setText(text);
}


// slots

void SceneDockWidget::OnContextMenu(const QPoint& point) {

    QMenu* mainMenu = new QMenu(this);

    static QVector<QPair<QString, UINT>> vecActorPair = {
        { "Actor", (UINT)Engine46::ActorType::Actor },
        { "SkyDome", (UINT)Engine46::ActorType::SkyDome },
        { "ParticleEmitter", (UINT)Engine46::ActorType::ParticleEmitter }
    };

    QMenu* subMenu = new QMenu("Create Actor", ui.treeView);
    connect(subMenu, &QMenu::triggered, this, &SceneDockWidget::CreateActor);
    for (const auto& pair : vecActorPair) {
        QAction* action = new QAction(pair.first, ui.treeView);
        action->setData(pair.second);
        subMenu->addAction(action);
    }
    mainMenu->addMenu(subMenu);

    static QVector<QPair<QString, UINT>> vecLightPair = {
        { "DirectionalLight", (UINT)Engine46::LightType::Directional },
        { "PointLight", (UINT)Engine46::LightType::Point },
        { "SpotLight", (UINT)Engine46::LightType::Spot }
    };

    subMenu = new QMenu("Create Light", ui.treeView);
    connect(subMenu, &QMenu::triggered, this, &SceneDockWidget::CreateLight);
    for (const auto& pair : vecLightPair) {
        QAction* action = new QAction(pair.first, ui.treeView);
        action->setData(pair.second);
        subMenu->addAction(action);
    }
    mainMenu->addMenu(subMenu);

    mainMenu->exec(mapToGlobal(point));
}

void SceneDockWidget::CreateActor(QAction* pAction) {
    Engine46::CActorManager* actorManager = Engine46::CGameSystem::GetGameSystem().GetActorManager();

    Engine46::CSceneBase* pScene = Engine46::CRendererSystem::GetRendererSystem().GetRenderScene();
    if (!pScene) return;

    QVariant variant = pAction->data();
    Engine46::ActorType Actortype = (Engine46::ActorType)variant.toInt();

    Engine46::CActorBase* pActor = actorManager->CreateActor(Actortype);
    if (!pActor) return;

    pScene->AddActorToScene(pActor);

    AddItemToTreeView(pActor);
}

void SceneDockWidget::CreateLight(QAction* pAction) {
    Engine46::CActorManager* actorManager = Engine46::CGameSystem::GetGameSystem().GetActorManager();

    Engine46::CSceneBase* pScene = Engine46::CRendererSystem::GetRendererSystem().GetRenderScene();
    if (!pScene) return;

    QVariant variant = pAction->data();
    Engine46::LightType LightType = (Engine46::LightType)variant.toInt();

    Engine46::CLight* pLight = actorManager->CreateLight(LightType);
    if (!pLight) return;

    pScene->AddActorToScene(pLight);

    AddItemToTreeView(pLight);
}

void SceneDockWidget::SelectActor(const QModelIndex& index) {
    QStandardItemModel* pItemModel = dynamic_cast<QStandardItemModel*>(ui.treeView->model());
    if (!pItemModel) return;

    pSelectItem = dynamic_cast<MyStandardItem*>(pItemModel->itemFromIndex(index));
    if (!pSelectItem) return;

    Engine46::CActorBase* pActor = pSelectItem->GetActor();
    if (!pActor) return;

    Engine46Editor* editor = dynamic_cast<Engine46Editor*>(parent());
    if (!editor) return;

    editor->SetSelectActor(pActor);
}

void SceneDockWidget::SelectItem(Engine46::CActorBase* pActor) {
    if (!pActor) return;

    QStandardItemModel* pItemModel = dynamic_cast<QStandardItemModel*>(ui.treeView->model());
    if (!pItemModel) return;

    QStandardItem* rootItem = pItemModel->item(0);
    if (!rootItem) return;

    int rowCount = rootItem->rowCount();
    for (int i = 0; i < rowCount; ++i) {
        MyStandardItem* pItem = dynamic_cast<MyStandardItem*>(rootItem->child(i, 0));
        if (!pItem) continue;

        if (pActor == pItem->GetActor()){
            pSelectItem = pItem;
            return;
        }
    }
}
