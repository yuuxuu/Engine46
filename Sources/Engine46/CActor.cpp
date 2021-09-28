/**
 * @file CObject.cpp
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#include "CActor.h"
#include "CGameSystem.h"
#include "CDataRecord.h"
#include "CConstantBuffer.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CTexture.h"
#include "CActorManager.h"
#include "CMaterialManager.h"
#include "CMeshManager.h"
#include "CShaderPackage.h"
#include "CShaderManager.h"
#include "CTextureManager.h"
#include "CRendererSystem.h"

#include "GraphicsAPI/DirectX12/CDX12Renderer.h"
#include "GraphicsAPI/DirectX12/CDX12ConstantBuffer.h"
#include "GraphicsAPI/DirectX12/CDX12ShaderPackage.h"

namespace Engine46 {

    // コンストラクタ
    CActorBase::CActorBase() :
        m_classID((int)ClassType::Root),
        m_actorID(0),
        m_actorName("Actor_" + std::to_string(m_actorID)),
        m_transform(Transform()),
        pParentActor(nullptr),
        m_parentActorID(-1)
    {
        m_actorName.resize(m_actorName.size());
    }

    // コンストラクタ
    CActorBase::CActorBase(const UINT classID, const char* actorName, const Transform transform) :
        m_classID(classID),
        m_actorID(0),
        m_actorName(actorName),
        m_transform(transform),
        pParentActor(nullptr),
        m_parentActorID(-1)
    {
        m_actorName.resize(m_actorName.size());
    }

    // デストラクタ
    CActorBase::~CActorBase()
    {}

    // 初期化
    void CActorBase::Initialize() {

        vecDataRecords.clear();

        vecDataRecords.emplace_back(CDataRecordBase(offsetof(CActorBase, m_classID), sizeof(m_classID)));
        vecDataRecords.emplace_back(CDataRecordBase(offsetof(CActorBase, m_actorID), sizeof(m_actorID)));
        vecDataRecords.emplace_back(CStrDataRecord(offsetof(CActorBase, m_actorName), m_actorName));
        vecDataRecords.emplace_back(CDataRecordBase(offsetof(CActorBase, m_transform), sizeof(m_transform)));
        vecDataRecords.emplace_back(CPtrDataRecord(m_parentActorID));
        vecDataRecords.emplace_back(CListDataRecord(m_childActorIDList));
    }

    // 更新
    void CActorBase::Update() {
        for (auto& chiled : pChildActorList) {
            chiled->Update();
        }
    }

    // 描画
    void CActorBase::Draw() {

        if (!pShaderPackage) return;

        pShaderPackage->SetShader();

        if (m_pWorldConstantBuffer) {
            Matrix matW = GetWorldMatrix();
            matW.dx_m = DirectX::XMMatrixTranspose(matW.dx_m);

            worldCB cb = {
                matW,
            };

            m_pWorldConstantBuffer->Update(&cb);
            m_pWorldConstantBuffer->Set((UINT)CB_TYPE::WORLD);
        }

        if (pMaterial) {
            pMaterial->Set((UINT)CB_TYPE::MATERIAL);
        }

        if (pMesh) {
            pMesh->Draw();
        }

        for (auto& chiled : pChildActorList) {
            chiled->Draw();
        }
    }

    // オブジェクトを保存
    bool CActorBase::Save(std::ofstream& ofs) {
        for (auto& record : vecDataRecords) {
            record.WriteData(ofs, (char*)this);
        }

        return true;
    }

    // オブジェクト読み込み
    bool CActorBase::Load(std::ifstream& ifs) {
        for (auto& record : vecDataRecords) {
            if (&record == &vecDataRecords[0]) continue;

            record.ReadData(ifs, (char*)this);
        }

        return true;
    }

    // コンスタントバッファを設定
    void CActorBase::SetWorldConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer) {
        if (pConstantBuffer) {
            m_pWorldConstantBuffer.swap(pConstantBuffer);
        }
    }

    // メッシュを設定
    void CActorBase::SetMesh(CMeshBase* pMesh) {
        if (pMesh) {
            this->pMesh = pMesh;
        }
    }

    // メッシュを設定
    void CActorBase::SetMesh(const char* meshName) {
        CMeshManager* meshManager = CGameSystem::GetGameSystem().GetMeshManager();
        meshManager->SetMeshToActor(this, meshName);
    }

    // マテリアルを設定
    void CActorBase::SetMaterial(CMaterialBase* pMaterial) {
        if (pMaterial) {
            this->pMaterial = pMaterial;
        }
    }

    // マテリアルを設定
    void CActorBase::SetMaterial(const char* materialName) {
        CMaterialManager* materialManager = CGameSystem::GetGameSystem().GetMaterialManager();
        materialManager->SetMaterialToActor(this, materialName);
    }

    // マテリアルにテクスチャを設定
    void CActorBase::SetTexture(CTextureBase* pTex) {
        if (pMaterial) {
            pMaterial->SetTexture(pTex);
        }
    }

    // マテリアルにテクスチャを設定
    void CActorBase::SetTexture(const char* textureName) {
        CTextureManager* textureManager = CGameSystem::GetGameSystem().GetTextureManager();
        textureManager->SetTextureToActor(this, textureName);
    }

    // シェーダーパッケージを設定
    void CActorBase::SetShaderPackage(CShaderPackage* pShaderPackage) {
        if (pShaderPackage) {
            this->pShaderPackage = pShaderPackage;
        }
    }

    // シェーダーパッケージを設定
    void CActorBase::SetShaderPackage(const char* shaderPackageName) {
        CShaderManager* shaderManager = CGameSystem::GetGameSystem().GetShaderManager();
        shaderManager->SetShaderPackageToActor(this, shaderPackageName);
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

        if (this->pParentActor) {
            this->DisconnectParentActor(this->pParentActor);
        }

        if (pParentActor) {
            m_parentActorID = pParentActor->m_actorID;
        }
        else {
            m_parentActorID = -1;
        }

        this->pParentActor = pParentActor;

        this->pParentActor->AddChiledActorList(this);
    }

    // 親アクターを解除
    void CActorBase::DisconnectParentActor(CActorBase* pParentActor) {
        if (pParentActor) {
            auto actorItr = std::find(pParentActor->pChildActorList.begin(), pParentActor->pChildActorList.end(), this);
            if (actorItr != pParentActor->pChildActorList.end()) {
                pParentActor->pChildActorList.erase(actorItr);
            }

            auto idItr = std::find(pParentActor->m_childActorIDList.begin(), pParentActor->m_childActorIDList.end(), m_actorID);
            if (idItr != pParentActor->m_childActorIDList.end()) {
                pParentActor->m_childActorIDList.erase(idItr);
            }
        }
    }

    // 子アクターを追加
    void CActorBase::AddChiledActorList(CActorBase* pChiledActor) {
        if (pChiledActor) {
            auto actorItr = std::find(pChildActorList.begin(), pChildActorList.end(), pChiledActor);
            if (actorItr == pChildActorList.end()) {
                pChildActorList.emplace_back(pChiledActor);
            }

            auto idItr = std::find(m_childActorIDList.begin(), m_childActorIDList.end(), pChiledActor->m_actorID);
            if (idItr == m_childActorIDList.end()) {
                m_childActorIDList.emplace_back(pChiledActor->m_actorID);
            }

            pChiledActor->ConnectParentActor(this);
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