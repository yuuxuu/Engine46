/**
 * @file CObject.cpp
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#include "CActor.h"
#include "CGameSystem.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CMaterialManager.h"
#include "CMeshManager.h"
#include "CShaderManager.h"
#include "CRendererSystem.h"
#include "CCamera.h"
#include "COBB.h"
#include "CModelMesh.h"
#include "CInput.h"

#include "CRenderer.h"

namespace Engine46 {

    // コンストラクタ
    CActorBase::CActorBase() :
        m_classID(int(ActorType::Actor)),
        m_actorID(0),
        m_actorName("Actor_" + std::to_string(m_actorID)),
        m_transform(Transform()),
        pParentActor(nullptr),
        m_parentActorID(-1),
        m_visible(true),
        m_billboradEnabled(false)
    {}

    // コンストラクタ
    CActorBase::CActorBase(const UINT classID, const std::string& actorName, const Transform& transform) :
        m_classID(classID),
        m_actorID(0),
        m_actorName(actorName),
        m_transform(transform),
        pParentActor(nullptr),
        m_parentActorID(-1),
        m_visible(true),
        m_billboradEnabled(false)
    {}

    // デストラクタ
    CActorBase::~CActorBase()
    {}

    // 初期化
    void CActorBase::Initialize() 
    {}

    // 更新
    void CActorBase::Update() {

        for (const auto& chiled : pChildActorList) {
            chiled->Update();
        }
    }

    // 描画
    void CActorBase::Draw() {

        if (!m_visible) return;

        if (pShaderPackage) {
            pShaderPackage->SetShader();

            CRendererBase* pRenderer = CRendererSystem::GetRendererSystem().GetRenderer();
            if (pRenderer) {
                pRenderer->SetSceneConstantBuffers((UINT)MyRS_Model::CBV_Camera);
            }

            Matrix matW = m_billboradEnabled ? GetBillboradMatrix() : GetWorldMatrix();
            matW.dx_m = DirectX::XMMatrixTranspose(matW.dx_m);

            worldCB cb = {
                matW,
            };
            UpdateWorldConstantBuffer(&cb);

            if (pMesh) {
                pMesh->Set();
                CMaterialBase* pMaterial = pMesh->GetMaterial();
                if (pMaterial) {
                    pMaterial->SetTexture((UINT)MyRS_Model::SRV_Diffuse);
                }
                pMesh->Draw();
            }

            if (pModelMesh) {
                if (pModelMesh) {
                    std::vector<CMeshBase*> vecMesh = pModelMesh->GetVecMesh();
                    for (const auto& mesh : vecMesh) {
                        mesh->Set();
                        CMaterialBase* pMaterial = mesh->GetMaterial();
                        if (pMaterial) {
                            pMaterial->SetTexture((UINT)MyRS_ModelLighting_Of_LightCulling::SRV_Diffuse);
                        }
                        mesh->Draw();
                    }
                }
            }
        }

        for (const auto& chiled : pChildActorList) {
            chiled->Draw();
        }
    }

    //　アクターをシリアライズ
    void CActorBase::SerializeActor(cereal::JSONOutputArchive& archive) {

        archive(cereal::make_nvp(m_actorName, *this));

        for (const auto& chiled : pChildActorList) {
            chiled->SerializeActor(archive);
        }
    }

    // アクターをデシリアライズ
    void CActorBase::DeserializeActor(cereal::JSONInputArchive& archive) {

        archive(
            cereal::make_nvp("ActorID", m_actorID),
            cereal::make_nvp("Transform", m_transform),
            cereal::make_nvp("visible", m_visible)
        );

        for (const auto& chiled : pChildActorList) {
            chiled->DeserializeActor(archive);
        }
    }

    // コンスタントバッファを更新
    void CActorBase::UpdateWorldConstantBuffer(void* pData) {
        if (m_pWorldConstantBuffer) {
            m_pWorldConstantBuffer->Update(pData);
            m_pWorldConstantBuffer->Set((UINT)CB_TYPE::WORLD);
        }
    }

    // コンスタントバッファを設定
    void CActorBase::SetWorldConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer) {
        if (pConstantBuffer) {
            m_pWorldConstantBuffer.swap(pConstantBuffer);
        }
    }

    // メッシュを設定
    void CActorBase::SetMesh(CMeshBase* pMesh) {
        this->pMesh = pMesh;
    }

    // メッシュを設定
    void CActorBase::SetMesh(const std::string& meshName) {
        CMeshManager* meshManager = CGameSystem::GetGameSystem().GetMeshManager();
        CMeshBase* pMesh = meshManager->CreateMesh(meshName);
        if (pMesh) {
            this->pMesh = pMesh;
        }
    }

    // モデルメッシュを設定
    void CActorBase::SetModelMesh(CModelMesh* pModelMesh) {
        this->pModelMesh = pModelMesh;
    }

    // モデルメッシュを設定
    void CActorBase::SetModelMesh(const std::string& modelName) {
        CMeshManager* meshManager = CGameSystem::GetGameSystem().GetMeshManager();
        CModelMesh* pModelMesh = meshManager->CreateModelMesh(modelName);
        if (pModelMesh) {
            this->pModelMesh = pModelMesh;
        }
    }

    // シェーダーパッケージを設定
    void CActorBase::SetShaderPackage(CShaderPackage* pShaderPackage) {
        if (pShaderPackage) {
            this->pShaderPackage = pShaderPackage;
        }
    }

    // シェーダーパッケージを設定
    void CActorBase::SetShaderPackage(const std::string& shaderPackageName) {
        CShaderManager* shaderManager = CGameSystem::GetGameSystem().GetShaderManager();
        CShaderPackage* pShaderPackage = shaderManager->CreateShaderPackage(shaderPackageName);
        if (pShaderPackage) {
            this->pShaderPackage = pShaderPackage;
        }
    }

    // アクターのOBB作成
    void CActorBase::CreateOBB() {

        if (!pMesh && !pModelMesh) return;

        m_pObb = std::make_unique<COBB>();
        m_pObb->Update(this);
        m_pObb->CreateOBBMesh(m_actorName + "_obbMesh");
    }

    // インプットを作成
    void CActorBase::CreateInput(){
        HWND hwnd = CGameSystem::GetGameSystem().GetHwnd();

        m_pInput = std::make_unique<CInput>(hwnd);

        HINSTANCE hInstance = GetModuleHandle(NULL);
        if (!m_pInput->Initialize(hInstance)) {
            m_pInput.release();
        }

        CInput* input = CGameSystem::GetGameSystem().GetInput();
        if (!input) {
            CGameSystem::GetGameSystem().SetInput(m_pInput.get());
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

    void CActorBase::SetVisible(bool visible) {
        m_visible = visible;

        if (pMesh) {
            pMesh->SetVisible(visible);
        }
        else {
            if (pModelMesh) {
                if (pModelMesh->GetVecMesh().empty()) return;

                MeshInfo meshInfo;
                for (const auto pMesh : pModelMesh->GetVecMesh()) {
                    pMesh->SetVisible(visible);
                }
            }
        }
    }

    // ワールド行列を取得
    Matrix CActorBase::GetWorldMatrix() {
        Matrix matScale;
        matScale.dx_m = DirectX::XMMatrixScaling(m_transform.scale.x, m_transform.scale.y, m_transform.scale.z);

        Matrix matRotate;
        matRotate.dx_m = DirectX::XMMatrixRotationRollPitchYaw(m_transform.rotate.x, m_transform.rotate.y, m_transform.rotate.z);

        Matrix matTrans;
        matTrans.dx_m = DirectX::XMMatrixTranslation(m_transform.pos.x, m_transform.pos.y, m_transform.pos.z);

        Matrix matWorld;
        matWorld.dx_m = matScale.dx_m * matRotate.dx_m * matTrans.dx_m;

        return matWorld;
    }

    Matrix CActorBase::GetBillboradMatrix() {
        Matrix matScale;
        matScale.dx_m = DirectX::XMMatrixScaling(m_transform.scale.x, m_transform.scale.y, m_transform.scale.z);

        Matrix matRotate;

        CSceneBase* pScene = CRendererSystem::GetRendererSystem().GetRenderScene();
        if (pScene) {
            CCamera* pCamera = pScene->GetCameraFromScene();
            if (pCamera) {
                matRotate = pCamera->GetInvViewMatrix();
            }
        }

        Matrix matTrans;
        matTrans.dx_m = DirectX::XMMatrixTranslation(m_transform.pos.x, m_transform.pos.y, m_transform.pos.z);

        Matrix matWorld;
        matWorld.dx_m = matScale.dx_m * matRotate.dx_m * matTrans.dx_m;

        return matWorld;
    }

    // 向きベクトルを取得
    VECTOR3 CActorBase::GetDirectionVector() {
        Matrix matRotate;
        matRotate.dx_m = DirectX::XMMatrixRotationRollPitchYaw(m_transform.rotate.x, m_transform.rotate.y, m_transform.rotate.z);

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