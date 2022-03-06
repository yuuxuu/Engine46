/**
 * @file CActor.h
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#pragma once

#ifndef _CACTOR_H_
#define _CACTOR_H_

#include "IObject.h"
#include "math.h"

namespace Engine46 {

    // 前方宣言
    class CConstantBufferBase;
    class CMeshBase;
    class CTextureBase;
    class CShaderPackage;
    class CInput;
    class COBB;
    class CModelMesh;

    enum class ActorType {
        Root,
        Actor,
        Camera,
        Light,
        SkyDome,
        ParticleEmitter,
    };

    class CActorBase : public IObject {
    protected:
        UINT                                    m_classID;

        UINT                                    m_actorID;

        std::string                             m_actorName;

        Transform                               m_transform;

        CMeshBase*                              pMesh;

        CModelMesh*                             pModelMesh;

        CShaderPackage*                         pShaderPackage;

        std::unique_ptr<CConstantBufferBase>    m_pWorldConstantBuffer;

        std::unique_ptr<COBB>                   m_pObb;

        std::unique_ptr<CInput>                 m_pInput;

        CActorBase*                             pParentActor;
        int                                     m_parentActorID;

        std::list<CActorBase*>                  pChildActorList;
        std::vector<int>                        m_childActorIDList;

        bool                                    m_visible;

        bool                                    m_billboradEnabled;

    public:
        template<class archive>
        void serialize(archive& ar) {
            ar(
                cereal::make_nvp("ActorID", m_actorID),
                cereal::make_nvp("Transform", m_transform),
                cereal::make_nvp("visible", m_visible)
            );
        }

    public:
        CActorBase();
        CActorBase(const UINT classID, const std::string& actorName, const Transform transform);
        virtual ~CActorBase();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void Draw() override;

        void SerializeActor(cereal::JSONOutputArchive& archive);
        void DeserializeActor(cereal::JSONInputArchive& archive);

        UINT GetClassID() const { return m_classID; }

        UINT GetActorID() const { return m_actorID; }
        void SetActorID(const UINT id) { m_actorID = id; }

        void SetActorName(const std::string& actorName) { m_actorName = actorName; }
        std::string GetActorName() const { return m_actorName; }

        CConstantBufferBase* GetWorldConstantBuffer() const { return m_pWorldConstantBuffer.get(); }
        void UpdateWorldConstantBuffer(void* pData);
        void SetWorldConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer);

        CMeshBase* GetMesh() const { return pMesh; }
        void SetMesh(CMeshBase* pMesh);
        void SetMesh(const std::string& meshName);

        CModelMesh* GetModelMesh() const { return pModelMesh; }
        void SetModelMesh(CModelMesh* pModelMesh);
        void SetModelMesh(const std::string& modelName);

        CShaderPackage* GetShaderPackage() const { return pShaderPackage; }
        void SetShaderPackage(CShaderPackage* pShaderPackage);
        void SetShaderPackage(const std::string& shaderPackageName);

        COBB* GetOBB() const { return m_pObb.get(); }
        void CreateOBB();

        CInput* GetInput() const { return m_pInput.get(); }
        void CreateInput();

        void ConnectParentActor(CActorBase* pParentActor);
        void DisconnectParentActor(CActorBase* pParentActor);

        CActorBase* GetParentActor() const { return pParentActor; }
        int GetParentActorID() const { return m_parentActorID; }

        void AddChiledActorList(CActorBase* pChiledObject);

        std::list<CActorBase*> GetChildActorList() const { return pChildActorList; }
        std::vector<int> GetChildActorIDList() const { return m_childActorIDList; }

        bool GetVisible() const { return m_visible; }
        void SetVisible(bool visible);

        bool GetBillBoardEnabled() const { return m_billboradEnabled; }
        void SetBillBoardEnabled(bool enabled) { m_billboradEnabled = enabled; }

        void SetTransform(const Transform& transform) { m_transform = transform; }
        Transform GetTransform() const { return m_transform; }

        void SetPos(const VECTOR3& pos) { m_transform.pos = pos; }
        VECTOR3 GetPos() const { return m_transform.pos; }

        void SetRotation(const VECTOR3& rotation) { m_transform.rotate = rotation; }
        VECTOR3 GetRotation() const { return m_transform.rotate; }

        void SetScale(const VECTOR3& scale) { m_transform.scale = scale; }
        VECTOR3 GetScale() const { return m_transform.scale; }

        Matrix GetWorldMatrix();
        Matrix GetBillboradMatrix();

        VECTOR3 GetDirectionVector();

        VECTOR3 GetRightVector();
        VECTOR3 GetUpVector();
        VECTOR3 GetForwardVector();
    };

} // namespace

#endif