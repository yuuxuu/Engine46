/**
 * @file Engine46SceneEditor.h
 * @brief
 * @author 木村優
 * @date 2021/06/26
 */

#include "Engine46SceneEditor.h"

#include <QHBoxLayout>
#include <QDebug>

#include "../Engine46/CRendererSystem.h"
#include "../Engine46/CScene.h"

// コンストラクタ
Engine46SceneEditor::Engine46SceneEditor(QWidget* parent)
	: QWidget(parent)
{
    ui.setupUi(this);

    ui.sceneRenderWidget->resize(QSize(1280, 720));

    ui.sceneTreeView->setMaximumWidth(200);
}

// デストラクタ
Engine46SceneEditor::~Engine46SceneEditor()
{}

// アクターの子アクターを再帰検索
void Engine46SceneEditor::RecursiveActor(Engine46::CActorBase* pRootActor, QStandardItemModel* pItemModel, QStandardItem* pRootItem){

    int rowIndex = 0;
    for (auto& pChildActor : pRootActor->GetChildActorList()) {
        QStandardItem* pChildItem = new QStandardItem(pChildActor->GetActorName().c_str());

        pRootItem->setChild(rowIndex++, 0, pChildItem);

        this->RecursiveActor(pChildActor, pItemModel, pChildItem);
    }
}

// 選択しているアイテムの名前変更を反映
void Engine46SceneEditor::ChangeValueReflectToName(const QString& string) {
    if (selectIndex.isValid()) {
        if (selectIndex.data().toString() == string) return;

        ui.sceneTreeView->model()->setData(selectIndex, string);
    }
}

// 選択しているアイテムを設定
void Engine46SceneEditor::SetSelectItem(const QModelIndex& index) {
    if (selectIndex != index) {
        selectIndex = index;
    }
}

// シーンツリービューの更新
void Engine46SceneEditor::UpdateSceneTreeView() {
    Engine46::CSceneBase* pScene = Engine46::CRendererSystem::GetRendererSystem().GetRenderScene();
    if (pScene) {
        Engine46::CActorBase* pRootActor = pScene->GetRootActor();

        if (pRootActor) {
            ui.sceneTreeView->reset();

            QStandardItemModel* pItemModel = new QStandardItemModel;
            QStandardItem* pRootItem = new QStandardItem(pRootActor->GetActorName().c_str());
            this->RecursiveActor(pRootActor, pItemModel, pRootItem);
            pItemModel->appendRow(pRootItem);

            QStringList stringList = {
                QString(pScene->GetSceneName().c_str())
            };
            pItemModel->setHorizontalHeaderLabels(stringList);

            ui.sceneTreeView->setModel(pItemModel);

            ui.sceneTreeView->setTreePosition(0);

            ui.sceneTreeView->expandAll();
        }
    }
}
