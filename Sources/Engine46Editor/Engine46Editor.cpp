#include "Engine46Editor.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFileSystemModel>
#include <QStandardItem>

#include "../Engine46/CFileSystem.h"
#include "../Engine46/CRendererSystem.h"
#include "../Engine46/CScene.h"
#include "../Engine46/CActor.h"

Engine46Editor::Engine46Editor(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    {
        pRenderWidget = new QWidget;
        pRenderWidget->resize(QSize(1080, 720));

        QHBoxLayout* horizonalLayout = new QHBoxLayout;
        horizonalLayout->addWidget(pRenderWidget);

        pSceneTreeView = new QTreeView;
        pSceneTreeView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        pSceneTreeView->setMaximumWidth(200);
        horizonalLayout->addWidget(pSceneTreeView);

        QVBoxLayout* verticalLayout = new QVBoxLayout(ui.centralWidget);
        verticalLayout->addLayout(horizonalLayout);

        pFileTreeView = new QTreeView;
        pFileTreeView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        pFileTreeView->setMaximumHeight(200);

        QFileSystemModel* fileModel = new QFileSystemModel;
        QModelIndex rootIndex = fileModel->setRootPath(Engine46::RESOURCE_ROOT_PATH);
        pFileTreeView->setModel(fileModel);
        pFileTreeView->setRootIndex(rootIndex);

        verticalLayout->addWidget(pFileTreeView);
    }
}

Engine46Editor::~Engine46Editor()
{}

void Engine46Editor::UpdateSceneTreeView() {
    Engine46::CSceneBase* pScene = Engine46::CRendererSystem::GetRendererSystem().GetRenderScene();

    QStandardItemModel* itemModel = new QStandardItemModel;

    Engine46::CActorBase* pRootActor = pScene->GetRootActor();
    
    QStandardItem* item = new QStandardItem(pRootActor->GetActorName());
    itemModel->appendRow(item);

    for (auto& pActor : pRootActor->GetChiledActorList()) {
        QStandardItem* chiledItem = new QStandardItem(pActor->GetActorName());

        itemModel->appendRow(chiledItem);
    }

    pSceneTreeView->setModel(itemModel);
}

void Engine46Editor::UpdateFileTreeView() {

}
