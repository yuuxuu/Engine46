﻿/**
 * @file CActorManager.cpp
 * @brief
 * @author 木村優
 * @date 2019/01/02
 */

#include "CActorManager.h"
#include "CSprite.h"
#include "CCamera.h"

#include "../Renderer/CDX11Renderer.h"

namespace Engine46 {

	constexpr const char* g_ActorListFileName = "ActorListData.bin";

	// コンストラクタ
	CActorManager::CActorManager(CDX11Renderer* pRenderer) :
		pDX11Renderer(pRenderer)
	{
		pRootActor = this->CreateActor((int)ClassType::Root);
	}

	// デストラクタ
	CActorManager::~CActorManager()
	{}

	// 初期化
	bool CActorManager::Initialize() {

		CActorBase* camera = this->CreateActor((int)ClassType::Camera);
		camera->Initialize();

		CActorBase* sprite = this->CreateActor((int)ClassType::Sprite);
		
		this->CreateMesh(sprite);
		this->CreateMaterial(sprite);
		this->CreateConstantBuffer(sprite);

		sprite->Initialize();

		return true;
	}

	// オブジェクト作成
	CActorBase* CActorManager::CreateActor(int classID) {
		std::unique_ptr<CActorBase> actor;
		RECT rect;

		switch ((ClassType)classID) {
		case ClassType::Root:
			actor = std::make_unique<CActorBase>();
			break;
		case ClassType::Camera:
			rect = pDX11Renderer->GetWindowRect();
			actor = std::make_unique<CCamera>(rect.w, rect.h);
			break;
		case ClassType::Sprite:
			actor = std::make_unique<CSprite>();
			break;
		}

		actor->CActorBase::Initialize();

		CActorBase* pActor = actor.get();

		if (classID != 0) {
			pRootActor->AddChiledActorList(pActor);
		}

		this->AddActorToActorVec(actor);

		return pActor;
	}

	// オブジェクトのメッシュを作成
	void CActorManager::CreateMesh(CActorBase* pActor) {
		if (pActor) {
			pActor->CreateMesh(pDX11Renderer);
		}
	}

	// オブジェクトのマテリアルを作成
	void CActorManager::CreateMaterial(CActorBase* pActor) {
		if (pActor) {
			pActor->CreateMaterial(pDX11Renderer);
		}
	}

	// オブジェクトのコンスタントバッファを作成
	void CActorManager::CreateConstantBuffer(CActorBase* pActor) {
		if (pActor) {
			pActor->CreateConstantBuffer(pDX11Renderer);
		}
	}

	// オブジェクトを保存
	bool CActorManager::SaveActor() {

		std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary;

		std::ofstream ofs;
		ofs.open(g_ActorListFileName, mode);

		if (!ofs.is_open()) return false;

		for (const auto& actor : m_pVecActor) {
			actor->Save(ofs);
		}

		return true;
	}

	// オブジェクトを読み込み
	bool CActorManager::LoadActor() {

		std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary;

		std::ifstream ifs;
		ifs.open(g_ActorListFileName, mode);

		if (!ifs.is_open()) return false;

		while (true) {

			int id = -1;
			ifs.read((char*)&id, sizeof(int));

			if (ifs.eof()) break;

			CActorBase* pActor = this->CreateActor(id);

			pActor->Load(ifs);
		}

		std::cout << g_ActorListFileName << "を読み込みしました。" << std::endl;

		this->ConnectActor();

		return true;
	}

	// オブジェクト同士の接続
	void CActorManager::ConnectActor() {
		for (const auto& actor : m_pVecActor) {
			int id = actor->GetParentActorID();
			if (id > -1) {
				actor->ConnectParentActor(m_pVecActor[id].get());
			}

			for (auto id : actor->GetChiledActorIDList()) {
				actor->AddChiledActorList(m_pVecActor[id].get());
			}
		}
	}

} // namespace