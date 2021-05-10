/**
 * @file CObject.cpp
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#include "CActor.h"
#include "CCamera.h"

#include "../Renderer/CDX11Renderer.h"

namespace Engine46 {

	static UINT g_ActorCount = 0;

	struct mainCB {
		Matrix	wvp;
		Matrix	lwvp;
		VECTOR3 cameraPos;
	};

	// コンストラクタ
	CActorBase::CActorBase() :
		m_ClassID((int)ClassType::Root),
		m_ActorID(g_ActorCount++),
		m_ActorName(),
		m_Transform(Transform()),
		pParentActor(nullptr),
		m_parentActorID(-1)
	{
		std::string str = "Object_" + std::to_string(m_ActorID);
		int size = (int)str.size() + 1;
		m_ActorName.reset(new char[size]);
		str.resize(size);
		str.copy(m_ActorName.get(), size);
	}

	// コンストラクタ
	CActorBase::CActorBase(const UINT id, const char* name, const Transform transform) :
		m_ClassID(id),
		m_ActorID(g_ActorCount++),
		m_ActorName(),
		m_Transform(transform),
		pParentActor(nullptr),
		m_parentActorID(-1)
	{
		std::string str = name + std::string('_' + std::to_string(m_ActorID));
		int size = (int)str.size() + 1;
		m_ActorName.reset(new char[size]);
		str.resize(size);
		str.copy(m_ActorName.get(), size);
	}

	// デストラクタ
	CActorBase::~CActorBase()
	{}

	// 初期化
	void CActorBase::Initialize() {

		vecDataRecords.clear();

		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CActorBase, m_ClassID), sizeof(m_ClassID)));
		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CActorBase, m_ActorID), sizeof(m_ActorID)));
		vecDataRecords.emplace_back(std::make_unique<CStrDataRecord>(offsetof(CActorBase, m_ActorName), m_ActorName));
		vecDataRecords.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CActorBase, m_Transform), sizeof(m_Transform)));
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

		if (m_pCb) {
			if (pParentActor) {
				for (auto& chiled : pParentActor->pChiledActorList) {
					if (chiled->m_ClassID == (UINT)ClassType::Camera) {
						CCamera* camera = dynamic_cast<CCamera*>(chiled);

						Matrix matW = this->GetWorldMatrix();

						Matrix matVP = camera->GetVPMatrix();

						Matrix matWVP;
						matWVP.dx_m = matW.dx_m * matVP.dx_m;
						Matrix matTranspose;
						matTranspose.dx_m = DirectX::XMMatrixTranspose(matWVP.dx_m);

						mainCB cb = {
							matTranspose,
							Matrix(),
							chiled->m_Transform.pos,
						};

						m_pCb->Update(&cb);

						m_pCb->Set(0);

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

	// コンスタントバッファの作成
	void CActorBase::CreateConstantBuffer(CDX11Renderer* pRenderer) {
		if (!m_pCb) {
			m_pCb = std::make_unique<CDX11CB>(pRenderer);

			m_pCb->CreateConstantBuffer(sizeof(mainCB));
		}
	}

	// メッシュ作成
	void CActorBase::CreateMesh(CDX11Renderer* pRenderer) {
		if (!m_pMesh) {
			m_pMesh = std::make_unique<CDX11Mesh>(pRenderer);
		}
	}

	// マテリアル作成
	void CActorBase::CreateMaterial(CDX11Renderer* pRenderer) {
		if (!m_pMaterial) {
			m_pMaterial = std::make_unique<CDX11Material>(pRenderer);
		}
	}

	// シェーダーパッケージを設定
	void CActorBase::SetShaderPackage(CShaderPackage* pShaderPackage) {
		if (m_pMaterial) {
			m_pMaterial->SetShaderPackage(pShaderPackage);
		}
	}

	// 親アクターを接続
	void CActorBase::ConnectParentActor(CActorBase* pParentActor) {

		if (this->pParentActor == pParentActor) return;

		this->pParentActor = pParentActor;

		if (pParentActor) {
			m_parentActorID = pParentActor->m_ActorID;
		}
		else {
			m_parentActorID = -1;
		}
	}

	// 子アクターを追加
	void CActorBase::AddChiledActorList(CActorBase* pChiledActor) {
		if (pChiledActor) {
			auto it = std::find(m_chiledActorIDList.begin(), m_chiledActorIDList.end(), pChiledActor->m_ActorID);

			if (it == m_chiledActorIDList.end()) {
				pChiledActor->ConnectParentActor(this);

				pChiledActorList.emplace_back(pChiledActor);

				m_chiledActorIDList.emplace_back(pChiledActor->m_ActorID);
			}
		}
	}

	// ワールド行列を取得
	Matrix CActorBase::GetWorldMatrix() {
		Matrix matScale;
		matScale.dx_m = DirectX::XMMatrixScaling(m_Transform.scale.x, m_Transform.scale.y, m_Transform.scale.z);
		
		Matrix matRotate;
		matRotate.dx_m = DirectX::XMMatrixRotationRollPitchYaw(m_Transform.rotation.x, m_Transform.rotation.y, m_Transform.rotation.z);
		
		Matrix matTrans;
		matTrans.dx_m = DirectX::XMMatrixTranslation(m_Transform.pos.x, m_Transform.pos.y, m_Transform.pos.z);

		Matrix matWorld;
		matWorld.dx_m = matScale.dx_m * matRotate.dx_m * matTrans.dx_m;

		return matWorld;
	}

} // namespace