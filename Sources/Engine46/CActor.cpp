﻿/**
 * @file CObject.cpp
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#include "CActor.h"
#include "CCamera.h"

namespace Engine46 {

	static UINT g_ActorCount = 0;

	struct mainCB {
		Matrix	wvp;
		Matrix	lwvp;
		VECTOR3 cameraPos;
	};

	// コンストラクタ
	CActorBase::CActorBase() :
		m_classID((int)ClassType::Root),
		m_actorID(g_ActorCount++),
		m_actorName(),
		m_transform(Transform()),
		pParentActor(nullptr),
		m_parentActorID(-1)
	{
		std::string str = "Object_" + std::to_string(m_actorID);
		int size = (int)str.size() + 1;
		m_actorName.reset(new char[size]);
		str.resize(size);
		str.copy(m_actorName.get(), size);
	}

	// コンストラクタ
	CActorBase::CActorBase(const UINT classID, const char* name, const Transform transform) :
		m_classID(classID),
		m_actorID(g_ActorCount++),
		m_actorName(),
		m_transform(transform),
		pParentActor(nullptr),
		m_parentActorID(-1)
	{
		std::string str = name + std::string('_' + std::to_string(m_actorID));
		int size = (int)str.size() + 1;
		m_actorName.reset(new char[size]);
		str.resize(size);
		str.copy(m_actorName.get(), size);
	}

	// デストラクタ
	CActorBase::~CActorBase()
	{}

	// 初期化
	void CActorBase::Initialize() {

		vecDataRecords.clear();

		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CActorBase, m_classID), sizeof(m_classID)));
		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CActorBase, m_actorID), sizeof(m_actorID)));
		vecDataRecords.emplace_back(std::make_unique<CStrDataRecord>(offsetof(CActorBase, m_actorName), m_actorName));
		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CActorBase, m_transform), sizeof(m_transform)));
		vecDataRecords.emplace_back(std::make_unique<CPtrDataRecord>(m_parentActorID));
		vecDataRecords.emplace_back(std::make_unique<CListDataRecord>(m_chiledActorIDList));
	}

	// 更新
	void CActorBase::Update() {
		for(auto& chiled : pChiledActorList) {
			chiled->Update();
		}
	}

	// 描画
	void CActorBase::Draw() {

		if (m_pMaterial) {
			m_pMaterial->Set(1);
		}

		if (m_pConstantBuffer) {
			if (pParentActor) {
				for (auto& chiled : pParentActor->pChiledActorList) {
					if (chiled->m_classID == (UINT)ClassType::Camera) {
						CCamera* camera = dynamic_cast<CCamera*>(chiled);

						Matrix matW = this->GetWorldMatrix();

						Matrix matVP = camera->GetViewProjectionMatrix();

						Matrix matWVP;
						matWVP.dx_m = matW.dx_m * matVP.dx_m;
						Matrix matTranspose;
						matTranspose.dx_m = DirectX::XMMatrixTranspose(matWVP.dx_m);

						mainCB cb = {
							matTranspose,
							Matrix(),
							chiled->m_transform.pos,
						};

						m_pConstantBuffer->Update(&cb);

						m_pConstantBuffer->Set(0);

						break;
					}
				}
			}
		}

		if (m_pMesh) {
			m_pMesh->Draw();
		}

		for (auto& chiled : pChiledActorList) {
			chiled->Draw();
		}
	}

	// オブジェクトを保存
	bool CActorBase::Save(std::ofstream& ofs) {
		for (auto& record : vecDataRecords) {
			record->WriteData(ofs, (char*)this);
		}

		return true;
	}

	// オブジェクト読み込み
	bool CActorBase::Load(std::ifstream& ifs) {
		for (auto& record : vecDataRecords) {
			if (&record == &vecDataRecords[0]) continue;

			record->ReadData(ifs, (char*)this);
		}

		return true;
	}

	// コンスタントバッファを作成
	void CActorBase::SetConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer) {
		if (!m_pConstantBuffer) {
			m_pConstantBuffer = std::move(pConstantBuffer);

			m_pConstantBuffer->CreateConstantBuffer(sizeof(mainCB));
		}
	}

	// メッシュを設定
	void CActorBase::SetMesh(CMeshBase* pMesh) {
		if (pMesh) {
			m_pMesh = pMesh;
		}
	}

	// マテリアルを設定
	void CActorBase::SetMaterial(CMaterialBase* pMaterial) {
		if (pMaterial) {
			m_pMaterial = pMaterial;
		}
	}

	// マテリアルにテクスチャを設定
	void CActorBase::SetTexture(CTextureBase* pTex) {
		if (m_pMaterial) {
			m_pMaterial->SetTexture(pTex);
		}
	}

	// シェーダーパッケージを設定
	void CActorBase::SetShaderPackage(CShaderPackage* pShaderPackage) {
		if (m_pMaterial) {
			m_pMaterial->SetShaderPackage(pShaderPackage);
		}
	}

	// インプットを設定
	void CActorBase::SetInput(CInput* pInput) {
		if (pInput) {
			this->pInput = pInput;
		}
	}

	// 親アクターを接続
	void CActorBase::ConnectParentActor(CActorBase* pParentActor) {

		if (this->pParentActor == pParentActor) return;

		this->pParentActor = pParentActor;

		if (pParentActor) {
			m_parentActorID = pParentActor->m_actorID;
		}
		else {
			m_parentActorID = -1;
		}
	}

	// 子アクターを追加
	void CActorBase::AddChiledActorList(CActorBase* pChiledActor) {
		if (pChiledActor) {
			auto it = std::find(m_chiledActorIDList.begin(), m_chiledActorIDList.end(), pChiledActor->m_actorID);

			if (it == m_chiledActorIDList.end()) {
				pChiledActor->ConnectParentActor(this);

				pChiledActorList.emplace_back(pChiledActor);

				m_chiledActorIDList.emplace_back(pChiledActor->m_actorID);
			}
		}
	}

	// ワールド行列を取得
	Matrix CActorBase::GetWorldMatrix() {
		Matrix matScale;
		matScale.dx_m = DirectX::XMMatrixScaling(m_transform.scale.x, m_transform.scale.y, m_transform.scale.z);
		
		Matrix matRotate;
		matRotate.dx_m = DirectX::XMMatrixRotationRollPitchYaw(m_transform.rotation.x, m_transform.rotation.y, m_transform.rotation.z);
		
		Matrix matTrans;
		matTrans.dx_m = DirectX::XMMatrixTranslation(m_transform.pos.x, m_transform.pos.y, m_transform.pos.z);

		Matrix matWorld;
		matWorld.dx_m = matScale.dx_m * matRotate.dx_m * matTrans.dx_m;

		return matWorld;
	}

	// 向きベクトルを取得
	VECTOR3 CActorBase::GetDirectionVector() {
		Matrix matRotate;
		matRotate.dx_m = DirectX::XMMatrixRotationRollPitchYaw(m_transform.rotation.x, m_transform.rotation.y, m_transform.rotation.z);

		VECTOR3 right;
		right.x = matRotate._11;
		right.y = matRotate._12;
		right.z = matRotate._13;
		float rd = Vec3Dot(VECTOR3(1.0f, 0.0f, 0.0f), right);

		VECTOR3 up;
		up.x = matRotate._21;
		up.y = matRotate._22;
		up.z = matRotate._23;
		float ud = Vec3Dot(VECTOR3(0.0f, 1.0f, 0.0f), up);

		VECTOR3 forward;
		forward.x = matRotate._31;
		forward.y = matRotate._32;
		forward.z = matRotate._33;
		float fd = Vec3Dot(VECTOR3(0.0f, 0.0f, 1.0f), forward);

		return VECTOR3(rd, ud, fd);

	}

	// 右方向ベクトルを取得
	VECTOR3 CActorBase::GetRightVector() {
		return GetDirectionVector() * VECTOR3(1.0f, 0.0f, 0.0f);
	}

	// 上方向ベクトルを取得
	VECTOR3 CActorBase::GetUpVector() {
		return GetDirectionVector() * VECTOR3(0.0f, 1.0f, 0.0f);
	}

	// 前方向ベクトルを取得
	VECTOR3 CActorBase::GetForwardVector() {
		return GetDirectionVector() * VECTOR3(0.0f, 0.0f, 1.0f);
	}

} // namespace