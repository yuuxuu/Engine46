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
    class CRendererBase;
    class CDataRecordBase;
    class CConstantBufferBase;
    class CMeshBase;
    class CMaterialBase;
    class CTextureBase;
    class CShaderPackage;
    class CInput;
    class CUnorderedAccessBufferBase;

    enum class ActorType {
        Root,
        Camera,
        Sprite,
        Light,
    };

    class CActorBase : public IObject {
    protected:
        std::vector<CDataRecordBase>            vecDataRecords;

        UINT                                    m_classID;

        UINT                                    m_actorID;

        std::string                             m_actorName;

        Transform                               m_transform;

        CMeshBase*                              pMesh;

        CMaterialBase*                          pMaterial;

        CShaderPackage*                         pShaderPackage;

        std::unique_ptr<CConstantBufferBase>    m_pWorldConstantBuffer;

        CInput*                                 pInput;

        CActorBase*                             pParentActor;
        int                                     m_parentActorID;

        std::list<CActorBase*>                  pChildActorList;
        std::vector<int>                        m_childActorIDList;

    public:
        CActorBase();
        CActorBase(const UINT classID, const std::string& actorName, const Transform transform);
        virtual ~CActorBase();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void Draw() override;

        virtual bool Save(std::ofstream& ofs) override;
        virtual bool Load(std::ifstream& ifs) override;

        virtual void InitializeResource(CRendererBase* pRenderer);

        void UpdateWorldConstantBuffer(void* pData);
        void SetWorldConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer);

        CMeshBase* GetMesh() const { return pMesh; }
        void SetMesh(CMeshBase* pMesh);
        void SetMesh(const std::string& meshName);

        CMaterialBase* GetMaterial() const { return pMaterial; }
        void SetMaterial(CMaterialBase* pMaterial);
        void SetMaterial(const std::string& materialName);

        void SetTexture(CTextureBase* pTex);
        void SetTexture(const std::string& textureName);

        CShaderPackage* GetShaderPackage() const { return pShaderPackage; }
        void SetShaderPackage(CShaderPackage* pShaderPackage);
        void SetShaderPackage(const std::string& shaderPackageName);

        void SetInput(CInput* pInput);

        void ConnectParentActor(CActorBase* pParentActor);
        void DisconnectParentActor(CActorBase* pParentActor);

        CActorBase* GetParentActor() const { return pParentActor; }
        int GetParentActorID() const { return m_parentActorID; }

        void AddChiledActorList(CActorBase* pChiledObject);

        std::list<CActorBase*> GetChildActorList() const { return pChildActorList; }
        std::vector<int> GetChildActorIDList() const { return m_childActorIDList; }

        UINT GetClassID() const { return m_classID; }

        void SetActorID(const UINT id) { m_actorID = id; }

        void SetActorName(const std::string& actorName) { m_actorName = actorName; }
        std::string GetActorName() const { return m_actorName.c_str(); }

        void SetTransform(const Transform& transform) { m_transform = transform; }
        Transform GetTransform() const { return m_transform; }

        void SetPos(const VECTOR3& pos) { m_transform.pos = pos; }
        VECTOR3 GetPos() const { return m_transform.pos; }

        void SetRotation(const VECTOR3& rotation) { m_transform.rotation = rotation; }
        VECTOR3 GetRotation() const { return m_transform.rotation; }

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