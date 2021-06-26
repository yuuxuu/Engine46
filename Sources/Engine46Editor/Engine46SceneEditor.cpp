/**
 * @file Engine46SceneEditor.h
 * @brief
 * @author 木村優
 * @date 2021/06/26
 */

#include "Engine46SceneEditor.h"

#include <QHBoxLayout>

#include "../Engine46/CRendererSystem.h"
#include "../Engine46/CScene.h"

// コンストラクタ
Engine46SceneEditor::Engine46SceneEditor(QWidget* parent)
	: QWidget(parent)
{
    QHBoxLayout* pHorizonalLayout = new QHBoxLayout(this);

    // シーンツリービュー
    pSceneTreeView = new QTreeView;
    pSceneTreeView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pSceneTreeView->setMaximumWidth(200);

    // レンダーウェジット
    pSceneRenderWidget = new QWidget;
    pSceneRenderWidget->resize(QSize(1080, 720));

    pHorizonalLayout->addWidget(pSceneTreeView);
    pHorizonalLayout->addWidget(pSceneRenderWidget);
}

// デストラクタ
Engine46SceneEditor::~Engine46SceneEditor()
{}

// アクターの子アクターを再帰検索
void Engine46SceneEditor::RecursiveActor(Engine46::CActorBase* pRootActor, QStandardItemModel* pItemModel, QStandardItem* pRootItem){

    int rowIndex = 0;
    for (auto& pChildActor : pRootActor->GetChildActorList()) {
        QStandardItem* pChildItem = new QStandardItem(pChildActor->GetActorName());

        pRootItem->setChild(rowIndex++, 0, pChildItem);

        this->RecursiveActor(pChildActor, pItemModel, pChildItem);
    }
}

// シーンツリービューの更新
void Engine46SceneEditor::UpdateSceneTreeView() {
    Engine46::CSceneBase* pScene = Engine46::CRendererSystem::GetRendererSystem().GetRenderScene();
    Engine46::CActorBase* pRootActor = pScene->GetRootActor();

    QStandardItemModel* pItemModel = new QStandardItemModel;
    QStandardItem* pRootItem = new QStandardItem(pRootActor->GetActorName());
    this->RecursiveActor(pRootActor, pItemModel, pRootItem);
    pItemModel->appendRow(pRootItem);

    QStringList stringList = {
        QString::fromLocal8Bit(pScene->GetSceneName())
    };
    pItemModel->setHorizontalHeaderLabels(stringList);

    pSceneTreeView->setModel(pItemModel);
}
