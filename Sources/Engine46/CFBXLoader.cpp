/**
 * @file CFBXLoader.cpp
 * @brief
 * @author 木村優
 * @date 2021/11/07
 */

#include "CFBXLoader.h"

#include "CMesh.h"
#include "CMaterial.h"
#include "CGameSystem.h"
#include "CMeshManager.h"
#include "CMaterialManager.h"
#include "CTextureManager.h"
#include "CModelMesh.h"
#include "CFileSystem.h"

namespace Engine46 {

    // コンストラクタ
    CFBXLoader::CFBXLoader()
    {}

    // デストラクタ
    CFBXLoader::~CFBXLoader()
    {}

    // FBXモデルの読み込み
    bool CFBXLoader::LoadModel(CModelMesh* pModel, const std::string& filePath) {

        if (!pModel) return false;

        FbxManager* fbxManager = FbxManager::Create();

        FbxIOSettings* fbxIOSetting = FbxIOSettings::Create(fbxManager, IOSROOT);

        FbxImporter* fbxInporter = FbxImporter::Create(fbxManager, "");
        if (!fbxInporter->Initialize(filePath.c_str(), -1, fbxManager->GetIOSettings())) {
            std::cout << filePath << "= 読み込み：失敗" << std::endl;
            return false;
        }

        FbxScene* fbxScene = FbxScene::Create(fbxManager, "");
        fbxInporter->Import(fbxScene);
        fbxInporter->Destroy();

        // 軸の設定
        FbxAxisSystem ourAxisSystem = FbxAxisSystem::DirectX;

        // DirectX系に変換
        FbxAxisSystem sceneAxisSystem = fbxScene->GetGlobalSettings().GetAxisSystem();
        if (ourAxisSystem != sceneAxisSystem) {
            FbxAxisSystem::DirectX.ConvertScene(fbxScene);
        }

        // 単位系の統一
        FbxSystemUnit SceneSystemUnit = fbxScene->GetGlobalSettings().GetSystemUnit();
        if (SceneSystemUnit.GetScaleFactor() != 1.0f)
        {
            // センチメーター単位にコンバートする。
            FbxSystemUnit::cm.ConvertScene(fbxScene);
        }

        FbxNode* pRootNode = fbxScene->GetRootNode();
        if (!pRootNode) return false;

        std::vector<FbxNode*> vecFbxMeshNode;

        RecursiveFbxNode(vecFbxMeshNode, pRootNode);

        for (const auto& pNode : vecFbxMeshNode) {
            FbxMesh* pFbxMesh = pNode->GetMesh();
            if (!pFbxMesh) continue;

            CMeshBase* pMesh = ConvertMesh(pFbxMesh);
            if (!pMesh) continue;

            int numMaterial = pNode->GetMaterialCount();
            for (int i = 0; i < numMaterial; ++i) {
                FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(i);
                if (!pFbxMaterial) continue;

                CMaterialBase* pMaterial = ConvertMaterial(pFbxMaterial);
                if (!pMaterial) continue;

                pMesh->AddMaterial(pMaterial);
            }

            pModel->AddMesh(pMesh);
        }

        return true;
    }

    // FbxNodeを再帰的に検索
    void CFBXLoader::RecursiveFbxNode(std::vector<FbxNode*>& outVecFbxNode, FbxNode* pRootNode) {
        int numChild = pRootNode->GetChildCount();

        for (int i = 0; i < numChild; ++i) {
            FbxNode* pChildNode = pRootNode->GetChild(i);
            if (!pChildNode) continue;

            FbxNodeAttribute* pFbxNodeAttribute = pChildNode->GetNodeAttribute();
            if (!pFbxNodeAttribute) continue;

            FbxNodeAttribute::EType type = pFbxNodeAttribute->GetAttributeType();
            if (type == FbxNodeAttribute::eMesh) {
                FbxMesh* pFbxMesh = pChildNode->GetMesh();
                if (!pFbxMesh) continue;

                outVecFbxNode.emplace_back(pChildNode);
            }

            RecursiveFbxNode(outVecFbxNode, pChildNode);
        }
    }

    // FbxMeshをコンバート
    CMeshBase* CFBXLoader::ConvertMesh(FbxMesh* pFbxMesh) {
        CMeshManager* meshManager = CGameSystem::GetGameSystem().GetMeshManager();
        if (!meshManager) return nullptr;

        CMeshBase* pMesh = meshManager->CreateMesh(pFbxMesh->GetName());
        if (!pMesh) return nullptr;

        std::cout << "メッシュ名 = " << pFbxMesh->GetName() << std::endl;

        int numVertex = pFbxMesh->GetControlPointsCount();
        std::cout << "総頂点数 = " << numVertex << std::endl;

        int numPolygon = pFbxMesh->GetPolygonCount();
        std::cout << "総ポリゴン数 = " << numPolygon << std::endl;

        int numIndex = pFbxMesh->GetPolygonVertexCount();
        std::cout << "総インデックス数 = " << numPolygon << std::endl;

        std::vector<DWORD> vecIndex;

        static const int indexOrder[] = {
            0, 1, 2,
            1, 2, 3, // 四角形
            2, 3, 4, // 五角形
            3, 4, 5, // 六角形
            4, 5, 6, // 七角形
            5, 6, 7, // 八角形
        };

        // インデックスの再構築中
        for (int i = 0; i < numPolygon; ++i) {
            int polygonSize = pFbxMesh->GetPolygonSize(i);

            int count = 3;
            if (polygonSize > 3) {
                count = (polygonSize - 2) * 3;
            }
            for (int j = 0; j < count; ++j) {
                int index = pFbxMesh->GetPolygonVertex(i, indexOrder[j]);
                vecIndex.emplace_back(index);
            }
        }

        std::vector<VertexInfo> vecVetexInfo(numVertex);

        FbxVector4* vertices = pFbxMesh->GetControlPoints();
        for (int i = 0; i < numVertex; ++i) {
            vecVetexInfo[i].vertex.x = static_cast<float>(vertices[i][0]);
            vecVetexInfo[i].vertex.y = static_cast<float>(vertices[i][1]);
            vecVetexInfo[i].vertex.z = static_cast<float>(vertices[i][2]);
        }

        int numNormalLayer = pFbxMesh->GetElementNormalCount();
        for (int i = 0; i < numNormalLayer; ++i) {
            FbxGeometryElementNormal* pFbxNormal = pFbxMesh->GetElementNormal(i);
            FbxGeometryElementTangent* pFbxTangent = pFbxMesh->GetElementTangent(i);
            FbxGeometryElementBinormal* pFbxBinormal = pFbxMesh->GetElementBinormal(i);

            int numNormal = pFbxNormal->GetDirectArray().GetCount();

            std::cout << "法線数 = " << numNormal << std::endl;

            FbxGeometryElement::EMappingMode mappingMode = pFbxNormal->GetMappingMode();
            FbxGeometryElement::EReferenceMode referenceMode = pFbxNormal->GetReferenceMode();

            if (mappingMode == FbxGeometryElement::eByControlPoint) {
                if (referenceMode == FbxGeometryElement::eDirect) {
                    for (int j = 0; j < numNormal; ++j) {
                        if (pFbxNormal) {
                            vecVetexInfo[j].normal.x = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(j)[0]);
                            vecVetexInfo[j].normal.y = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(j)[1]);
                            vecVetexInfo[j].normal.z = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(j)[2]);
                        }
                        if (pFbxTangent) {
                            vecVetexInfo[j].tangent.x = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(j)[0]);
                            vecVetexInfo[j].tangent.y = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(j)[1]);
                            vecVetexInfo[j].tangent.z = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(j)[2]);
                        }
                        if (pFbxBinormal) {
                            vecVetexInfo[j].binormal.x = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(j)[0]);
                            vecVetexInfo[j].binormal.y = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(j)[1]);
                            vecVetexInfo[j].binormal.z = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(j)[2]);
                        }
                    }
                }
            }
            else if (mappingMode == FbxGeometryElement::eByPolygonVertex) {
                if (referenceMode == FbxGeometryElement::eDirect) {
                    for (int j = 0; j < numVertex; ++j) {
                        if (pFbxNormal) {
                            vecVetexInfo[j].normal.x = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(j)[0]);
                            vecVetexInfo[j].normal.y = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(j)[1]);
                            vecVetexInfo[j].normal.z = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(j)[2]);
                        }
                        if (pFbxTangent) {
                            vecVetexInfo[j].tangent.x = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(j)[0]);
                            vecVetexInfo[j].tangent.y = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(j)[1]);
                            vecVetexInfo[j].tangent.z = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(j)[2]);
                        }
                        if (pFbxBinormal) {
                            vecVetexInfo[j].binormal.x = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(j)[0]);
                            vecVetexInfo[j].binormal.y = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(j)[1]);
                            vecVetexInfo[j].binormal.z = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(j)[2]);
                        }
                    }
                }
                else if (referenceMode == FbxGeometryElement::eIndexToDirect) {
                    int numNormalIndex = pFbxNormal->GetIndexArray().GetCount();

                    for (int j = 0; j < numNormalIndex; ++j) {
                        int index = static_cast<int>(pFbxNormal->GetIndexArray().GetAt(j));

                        if (pFbxNormal) {
                            vecVetexInfo[vecIndex[j]].normal.x = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(index)[0]);
                            vecVetexInfo[vecIndex[j]].normal.y = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(index)[1]);
                            vecVetexInfo[vecIndex[j]].normal.z = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(index)[2]);
                        }
                        if (pFbxTangent) {
                            vecVetexInfo[vecIndex[j]].tangent.x = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(index)[0]);
                            vecVetexInfo[vecIndex[j]].tangent.y = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(index)[1]);
                            vecVetexInfo[vecIndex[j]].tangent.z = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(index)[2]);
                        }
                        if (pFbxBinormal) {
                            vecVetexInfo[vecIndex[j]].binormal.x = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(index)[0]);
                            vecVetexInfo[vecIndex[j]].binormal.y = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(index)[1]);
                            vecVetexInfo[vecIndex[j]].binormal.z = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(index)[2]);
                        }
                    }
                }
            }
        }

        int numColorLayer = pFbxMesh->GetElementVertexColorCount();
        for (int i = 0; i < numColorLayer; ++i) {
            FbxGeometryElementVertexColor* pFbxColor = pFbxMesh->GetElementVertexColor(i);

            int numColor = pFbxColor->GetDirectArray().GetCount();
            std::cout << "頂点カラー数 = " << numColor << std::endl;

            FbxGeometryElement::EMappingMode mappingMode = pFbxColor->GetMappingMode();
            FbxGeometryElement::EReferenceMode referenceMode = pFbxColor->GetReferenceMode();

            if (mappingMode == FbxGeometryElement::eByControlPoint) {
                if (referenceMode == FbxGeometryElement::eDirect) {
                    for (int j = 0; j < numColor; ++j) {
                        vecVetexInfo[j].color.x = static_cast<float>(pFbxColor->GetDirectArray().GetAt(j)[0]);
                        vecVetexInfo[j].color.y = static_cast<float>(pFbxColor->GetDirectArray().GetAt(j)[1]);
                        vecVetexInfo[j].color.z = static_cast<float>(pFbxColor->GetDirectArray().GetAt(j)[2]);
                        vecVetexInfo[j].color.w = static_cast<float>(pFbxColor->GetDirectArray().GetAt(j)[3]);
                    }
                }
            }
            else if (mappingMode == FbxGeometryElement::eByPolygonVertex) {
                if (referenceMode == FbxGeometryElement::eDirect) {
                    for (int j = 0; j < numVertex; ++j) {
                        vecVetexInfo[j].color.x = static_cast<float>(pFbxColor->GetDirectArray().GetAt(j)[0]);
                        vecVetexInfo[j].color.y = static_cast<float>(pFbxColor->GetDirectArray().GetAt(j)[1]);
                        vecVetexInfo[j].color.z = static_cast<float>(pFbxColor->GetDirectArray().GetAt(j)[2]);
                        vecVetexInfo[j].color.w = static_cast<float>(pFbxColor->GetDirectArray().GetAt(j)[3]);
                    }
                }
                else if (referenceMode == FbxGeometryElement::eIndexToDirect) {
                    int numColorIndex = pFbxColor->GetIndexArray().GetCount();
                    
                    for (int j = 0; j < numColorIndex; ++j) {
                        int index = static_cast<int>(pFbxColor->GetIndexArray().GetAt(j));

                        vecVetexInfo[vecIndex[j]].color.x = static_cast<float>(pFbxColor->GetDirectArray().GetAt(index)[0]);
                        vecVetexInfo[vecIndex[j]].color.y = static_cast<float>(pFbxColor->GetDirectArray().GetAt(index)[1]);
                        vecVetexInfo[vecIndex[j]].color.z = static_cast<float>(pFbxColor->GetDirectArray().GetAt(index)[2]);
                        vecVetexInfo[vecIndex[j]].color.w = static_cast<float>(pFbxColor->GetDirectArray().GetAt(index)[3]);
                    }
                }
            }
        }

        int numUVLayer = pFbxMesh->GetElementUVCount();
        for (int i = 0; i < numUVLayer; ++i) {
            FbxGeometryElementUV* pFbxUV = pFbxMesh->GetElementUV(i);

            int numUV = pFbxUV->GetDirectArray().GetCount();
            std::cout << "頂点UV数 = " << numUV << std::endl;

            std::cout << "UVセット名 = " << pFbxUV->GetName() << std::endl;

            FbxGeometryElement::EMappingMode mappingMode = pFbxUV->GetMappingMode();
            FbxGeometryElement::EReferenceMode referenceMode = pFbxUV->GetReferenceMode();

            if (mappingMode == FbxGeometryElement::eByControlPoint) {
                if (referenceMode == FbxGeometryElement::eDirect) {
                    for (int j = 0; j < numUV; ++j) {
                        vecVetexInfo[j].uv.x = static_cast<float>(pFbxUV->GetDirectArray().GetAt(j)[0]);
                        vecVetexInfo[j].uv.y = 1.0f - static_cast<float>(pFbxUV->GetDirectArray().GetAt(j)[1]);
                    }
                }
                else if (referenceMode == FbxGeometryElement::eIndexToDirect) {
                    for (int j = 0; j < numUV; ++j) {
                        int index = static_cast<int>(vecIndex[j]);

                        vecVetexInfo[j].uv.x = static_cast<float>(pFbxUV->GetDirectArray().GetAt(index)[0]);
                        vecVetexInfo[j].uv.y = 1.0f - static_cast<float>(pFbxUV->GetDirectArray().GetAt(index)[1]);
                    }
                }
            }
            else if (mappingMode == FbxGeometryElement::eByPolygonVertex) {
                if (referenceMode == FbxGeometryElement::eDirect) {
                    for (int j = 0; j < numUV; ++j) {
                        vecVetexInfo[j].uv.x = static_cast<float>(pFbxUV->GetDirectArray().GetAt(j)[0]);
                        vecVetexInfo[j].uv.y = 1.0f - static_cast<float>(pFbxUV->GetDirectArray().GetAt(j)[1]);
                    }
                }
                else if (referenceMode == FbxGeometryElement::eIndexToDirect) {
                    int numUVIndex = pFbxUV->GetIndexArray().GetCount();
                    
                    for (int j = 0; j < numUVIndex; ++j) {
                        int index = static_cast<int>(pFbxUV->GetIndexArray().GetAt(j));

                        vecVetexInfo[vecIndex[j]].uv.x = static_cast<float>(pFbxUV->GetDirectArray().GetAt(index)[0]);
                        vecVetexInfo[vecIndex[j]].uv.y = 1.0f - static_cast<float>(pFbxUV->GetDirectArray().GetAt(index)[1]);
                    }
                }
            }
        }

        pMesh->CreateVertexBuffer(PRIMITIVE_TOPOLOGY_TYPE::TRIANGLELIST, vecVetexInfo);
        pMesh->CreateIndexBuffer(vecIndex);

        pMesh->SetMeshInfo(vecVetexInfo);

        return pMesh;
    }

    // FbxMaterialをコンバート
    CMaterialBase* CFBXLoader::ConvertMaterial(FbxSurfaceMaterial* pFbxMaterial) {
        CMaterialManager* materialManager = CGameSystem::GetGameSystem().GetMaterialManager();
        if (!materialManager) return nullptr;

        CMaterialBase* pMaterial = materialManager->CreateMaterial(pFbxMaterial->GetName());
        if (!pMaterial) return nullptr;

        std::cout << "マテリアル名 = " << pFbxMaterial->GetName() << std::endl;


        if (std::string("PictureBorder") == pFbxMaterial->GetName()) {
            std::cout << "マテリアル名 = " << pFbxMaterial->GetName() << std::endl;
        }

        if (pFbxMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
            FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(pFbxMaterial);

            VECTOR4 diffuse;
            diffuse.x = static_cast<float>(lambert->Diffuse.Get()[0]);
            diffuse.y = static_cast<float>(lambert->Diffuse.Get()[1]);
            diffuse.z = static_cast<float>(lambert->Diffuse.Get()[2]);
            diffuse.w = 1.0f;

            VECTOR4 ambient;
            ambient.x = static_cast<float>(lambert->Ambient.Get()[0]);
            ambient.y = static_cast<float>(lambert->Ambient.Get()[1]);
            ambient.z = static_cast<float>(lambert->Ambient.Get()[2]);
            ambient.w = 1.0f;

            VECTOR4 emissive;
            emissive.x = static_cast<float>(lambert->Emissive.Get()[0]);
            emissive.y = static_cast<float>(lambert->Emissive.Get()[1]);
            emissive.z = static_cast<float>(lambert->Emissive.Get()[2]);
            emissive.w = 1.0f;
            
            pMaterial->SetDiffuse(diffuse);
            pMaterial->SetAmbient(ambient);
            pMaterial->SetEmissive(emissive);
        }
        else if (pFbxMaterial->GetClassId().Is(FbxSurfacePhong::ClassId)) {
            FbxSurfacePhong* phong = static_cast<FbxSurfacePhong*>(pFbxMaterial);

            VECTOR4 diffuse;
            diffuse.x = static_cast<float>(phong->Diffuse.Get()[0]);
            diffuse.y = static_cast<float>(phong->Diffuse.Get()[1]);
            diffuse.z = static_cast<float>(phong->Diffuse.Get()[2]);
            diffuse.w = 1.0f;

            VECTOR4 specular;
            specular.x = static_cast<float>(phong->Specular.Get()[0]);
            specular.y = static_cast<float>(phong->Specular.Get()[1]);
            specular.z = static_cast<float>(phong->Specular.Get()[2]);
            specular.w = 1.0f;

            VECTOR4 ambient;
            ambient.x = static_cast<float>(phong->Ambient.Get()[0]);
            ambient.y = static_cast<float>(phong->Ambient.Get()[1]);
            ambient.z = static_cast<float>(phong->Ambient.Get()[2]);
            ambient.w = 1.0f;

            VECTOR4 emissive;
            emissive.x = static_cast<float>(phong->Emissive.Get()[0]);
            emissive.y = static_cast<float>(phong->Emissive.Get()[1]);
            emissive.z = static_cast<float>(phong->Emissive.Get()[2]);
            emissive.w = 1.0f;

            pMaterial->SetDiffuse(diffuse);
            pMaterial->SetSpecular(specular);
            pMaterial->SetAmbient(ambient);
            pMaterial->SetEmissive(emissive);
        }

        CTextureManager* textureManager = CGameSystem::GetGameSystem().GetTextureManager();
        if (!textureManager) return pMaterial;

        FbxProperty pFbxProp = pFbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

        int numTexture = pFbxProp.GetSrcObjectCount<FbxFileTexture>();

        for (int i = 0; i < numTexture; ++i) {
            FbxFileTexture* pFbxTexture = pFbxProp.GetSrcObject<FbxFileTexture>(i);
            if (!pFbxTexture) continue;

            std::cout << "テクスチャ名 = " << pFbxTexture->GetRelativeFileName() << std::endl;

            const UINT charSize = 128;

            char drive[charSize];
            char dir[charSize];
            char name[charSize];
            char extension[charSize];

            _splitpath_s(pFbxTexture->GetRelativeFileName(), drive, dir, name, extension);

            std::string filename(name);
            filename += extension;

            FileInfo* pFileInfo = CFileSystem::GetFileSystem().GetFileInfoFromMap(filename.c_str());

            if (!pFileInfo) {
                filename = name;
                filename += +".jpg";
            }

            CTextureBase* pTexture = textureManager->CreateTexture(filename.c_str());
            if (!pTexture) continue;

            pMaterial->AddTexture(pTexture);
        }

        return pMaterial;
    }

} // namespace
