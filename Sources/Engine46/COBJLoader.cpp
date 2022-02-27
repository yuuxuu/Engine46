/**
 * @file COBJLoader.cpp
 * @brief
 * @author 木村優
 * @date 2022/01/30
 */

#include "COBJLoader.h"

#include "math.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CModelMesh.h"

#include "CGameSystem.h"
#include "CFileManager.h"
#include "CMeshManager.h"
#include "CMaterialManager.h"
#include "CTextureManager.h"
#include "CFileManager.h"

namespace Engine46 {

    // コンストラクタ
    COBJLoader::COBJLoader()
    {}

    // デストラクタ
    COBJLoader::~COBJLoader()
    {}

    // モデルの読み込み
    bool COBJLoader::LoadModel(CModelMesh* pModel, const std::string& filePath) {
        std::ifstream ifs;

        ifs.open(filePath);
        if (!ifs) return false;

        if (!LoadMesh(pModel, ifs)) return false;

        return true;
    }

    bool COBJLoader::LoadMesh(CModelMesh* pModel, std::ifstream& ifs) {
        CMeshManager* meshManager = CGameSystem::GetGameSystem().GetMeshManager();
        if (!meshManager) return false;

        std::string meshName;
        std::string newMeshName;
        std::string materialName;
        std::string newMaterialName;
        std::string mtlLibName;

        std::vector<VECTOR3> vecVertex;
        std::vector<VECTOR3> vecNormal;
        std::vector<VECTOR2> vecUV;

        std::vector<DWORD> vecVertexIndices;
        std::vector<DWORD> vecNormalIndices;
        std::vector<DWORD> vecUVIndices;

        std::string lineStr;
        while (!ifs.eof()) {
            std::getline(ifs, lineStr);
            if (lineStr.empty()) continue;

            static const std::vector<std::string> findKeyStrs = {
               "mtllib ",
               "g ",
               "usemtl ",
            };
            for (const auto& findKey : findKeyStrs) {
                std::string key = lineStr.substr(0, findKey.size());

                if (key == "mtllib ") {
                    if (mtlLibName.empty()) {
                        mtlLibName = lineStr.substr(key.size(), lineStr.size() - key.size());
                        if (!LoadMaterial(mtlLibName)) return false;
                    }
                    break;
                }
                else if (key == "g ") {
                    newMeshName = lineStr.substr(key.size(), lineStr.size() - key.size());

                    if (!meshName.empty() && meshName != newMeshName) {
                        int numIndex = static_cast<int>(vecVertexIndices.size());
                        if (numIndex > 0) {
                            std::vector<VertexInfo> vecVertexInfo(numIndex);

                            for (int i = 0; i < numIndex; ++i) {
                                if (!vecVertexIndices.empty()) {
                                    int index = vecVertexIndices[i];
                                    vecVertexInfo[i].vertex = vecVertex[index];
                                }

                                if (!vecNormalIndices.empty()) {
                                    int index = vecNormalIndices[i];
                                    vecVertexInfo[i].normal = vecNormal[index];
                                }

                                if (!vecUVIndices.empty()) {
                                    int index = vecUVIndices[i];
                                    vecVertexInfo[i].uv = vecUV[index];
                                }
                            }

                            vecVertexIndices.clear();
                            vecNormalIndices.clear();
                            vecUVIndices.clear();

                            CMeshBase* pMesh = meshManager->CreateMesh(meshName);

                            pMesh->CreateVertexBuffer(PRIMITIVE_TOPOLOGY_TYPE::TRIANGLELIST, vecVertexInfo);

                            pMesh->SetMeshInfo(vecVertexInfo);

                            pMesh->SetMaterial(materialName);

                            pModel->AddMesh(pMesh);
                        }
                    }
                    meshName = newMeshName;
                    break;
                }
                else if (key == "usemtl ") {
                    newMaterialName = lineStr.substr(key.size(), lineStr.size() - key.size());

                    if (!materialName.empty() && materialName != newMaterialName) {
                        int numIndex = static_cast<int>(vecVertexIndices.size());
                        if (numIndex > 0) {
                            std::vector<VertexInfo> vecVertexInfo(numIndex);

                            for (int i = 0; i < numIndex; ++i) {
                                if (!vecVertexIndices.empty()) {
                                    int index = vecVertexIndices[i];
                                    vecVertexInfo[i].vertex = vecVertex[index];
                                }

                                if (!vecNormalIndices.empty()) {
                                    int index = vecNormalIndices[i];
                                    vecVertexInfo[i].normal = vecNormal[index];
                                }

                                if (!vecUVIndices.empty()) {
                                    int index = vecUVIndices[i];
                                    vecVertexInfo[i].uv = vecUV[index];
                                }
                            }

                            vecVertexIndices.clear();
                            vecNormalIndices.clear();
                            vecUVIndices.clear();

                            CMeshBase* pMesh = meshManager->CreateMesh(meshName + materialName);

                            pMesh->CreateVertexBuffer(PRIMITIVE_TOPOLOGY_TYPE::TRIANGLELIST, vecVertexInfo);

                            pMesh->SetMeshInfo(vecVertexInfo);

                            pMesh->SetMaterial(materialName);

                            pModel->AddMesh(pMesh);
                        }
                    }
                    materialName = newMaterialName;
                    break;
                }
            }

            static const std::vector<std::string> keyStrs = {
                "v ",
                "vn ",
                "vt ",
                "f ",
            };
            for (const auto& key : keyStrs) {
                std::string bufStr = lineStr.substr(0, key.size());
                if (bufStr == "v ") {
                    std::stringstream ss(lineStr.substr(key.size()));

                    VECTOR3 vertex;
                    ss >> vertex.x >> vertex.y >> vertex.z;

                    vecVertex.emplace_back(vertex);
                    break;
                }
                else if (bufStr == "vn ") {
                    std::stringstream ss(lineStr.substr(key.size()));

                    VECTOR3 normal;
                    ss >> normal.x >> normal.y >> normal.z;

                    vecNormal.emplace_back(normal);
                    break;
                }
                else if (bufStr == "vt ") {
                    std::stringstream ss(lineStr.substr(key.size()));

                    VECTOR2 uv;
                    ss >> uv.x >> uv.y;

                    vecUV.emplace_back(uv);
                    break;
                }
                else if (bufStr == "f ") {
                    std::stringstream ss(lineStr.substr(key.size()));

                    std::vector<DWORD> vecVertexIndex;
                    std::vector<DWORD> vecNormalIndex;
                    std::vector<DWORD> vecUVIndex;

                    std::string bufStr;
                    const int typeCnt = 3;
                    while (std::getline(ss, bufStr, ' ')) {
                        if (bufStr.empty()) continue;

                        bufStr += "/";

                        int indedxType[typeCnt] = { -1, -1, -1 };
                        int numberCnt = 0;
                        int size = static_cast<int>(bufStr.size());
                        int start = 0;
                        int end = 0;
                        for (int i = 0; i < size; ++i) {
                            std::string numberStr = bufStr.substr(i, 1);
                            std::string nextNumberStr = bufStr.substr(i + 1, 1);
                            if (numberStr == "/" && nextNumberStr == "/") {
                                numberCnt++;
                                i++;
                                continue;
                            }
                            else if (numberStr == "/") {
                                end = i;

                                std::string str = bufStr.substr(start, end - start);

                                indedxType[numberCnt++] = std::stoi(str) - 1;

                                start = i + 1;
                            }
                        }
                        for (int i = 0; i < typeCnt; ++i) {
                            if (indedxType[i] < 0) {
                                continue;
                            }
                            if (i == 0) {
                                vecVertexIndex.push_back(indedxType[i]);
                            }
                            else if (i == 1) {
                                vecUVIndex.push_back(indedxType[i]);
                            }
                            else if (i == 2) {
                                vecNormalIndex.push_back(indedxType[i]);
                            }
                        }
                    }
                    int vertexCnt = static_cast<int>(vecVertexIndex.size());
                    if (vertexCnt == 3) {
                        for (int i = 0; i < vertexCnt; ++i) {
                            if (!vecVertexIndex.empty()) {
                                vecVertexIndices.push_back(vecVertexIndex[i]);
                            }
                            if (!vecNormalIndex.empty()) {
                                vecNormalIndices.push_back(vecNormalIndex[i]);
                            }
                            if (!vecUVIndex.empty()) {
                                vecUVIndices.push_back(vecUVIndex[i]);
                            }
                        }
                    }
                    else if (vertexCnt == 4) {
                        static const int indexOrder[] = {
                            0, 1, 2,
                            0, 2, 3
                        };
                        for (auto index : indexOrder) {
                            if (!vecVertexIndex.empty()) {
                                vecVertexIndices.push_back(vecVertexIndex[index]);
                            }
                            if (!vecNormalIndex.empty()) {
                                vecNormalIndices.push_back(vecNormalIndex[index]);
                            }
                            if (!vecUVIndex.empty()) {
                                vecUVIndices.push_back(vecUVIndex[index]);
                            }
                        }
                    }
                    break;
                }
            }
        }
        if (!meshName.empty()) {
            int numIndex = static_cast<int>(vecVertexIndices.size());
            if (numIndex > 0) {
                std::vector<VertexInfo> vecVertexInfo(numIndex);

                for (int i = 0; i < numIndex; ++i) {
                    if (!vecVertexIndices.empty()) {
                        int index = vecVertexIndices[i];
                        vecVertexInfo[i].vertex = vecVertex[index];
                    }

                    if (!vecNormalIndices.empty()) {
                        int index = vecNormalIndices[i];
                        vecVertexInfo[i].normal = vecNormal[index];
                    }

                    if (!vecUVIndices.empty()) {
                        int index = vecUVIndices[i];
                        vecVertexInfo[i].uv = vecUV[index];
                    }
                }

                CMeshBase* pMesh = meshManager->CreateMesh(meshName);

                pMesh->CreateVertexBuffer(PRIMITIVE_TOPOLOGY_TYPE::TRIANGLELIST, vecVertexInfo);

                pMesh->SetMeshInfo(vecVertexInfo);

                pMesh->SetMaterial(materialName);

                pModel->AddMesh(pMesh);
            }
        }

        return true;
    }

    bool COBJLoader::LoadMaterial(const std::string& fileName) {

        FileInfo* pFileInfo = CFileManager::GetFileManager().GetFileInfoFromMap(fileName);
        if (pFileInfo) {
            std::ifstream ifs;

            ifs.open(pFileInfo->filePath);
            if (!ifs) return false;

            CMaterialManager* materialManager = CGameSystem::GetGameSystem().GetMaterialManager();
            if (!materialManager) return false;

            std::string materialName;
            std::string findKeyStr("newmtl ");

            std::string diffuseTexStr;

            VECTOR4 diffuse;
            VECTOR4 ambient;
            VECTOR4 specular;

            std::string lineStr;
            while (!ifs.eof()) {
                std::getline(ifs, lineStr);
                if (lineStr.empty()) continue;

                std::string str = lineStr.substr(0, findKeyStr.size());
                if (findKeyStr == str) {
                    if (!materialName.empty()) {
                        CMaterialBase* pMaterial = materialManager->CreateMaterial(materialName);
                        if (!pMaterial) continue;

                        pMaterial->SetDiffuse(diffuse);
                        pMaterial->SetAmbient(ambient);
                        pMaterial->SetSpecular(specular);
                        pMaterial->SetTexture(diffuseTexStr);
                    }
                    materialName = lineStr.substr(findKeyStr.size(), lineStr.size() - findKeyStr.size());
                }

                std::stringstream ss(lineStr);

                std::string bufStr;
                while (std::getline(ss, bufStr, '\t')) {

                    static const std::vector<std::string> keyStrs = {
                        "Ka ",
                        "Kd ",
                        "Ks ",
                        "map_Kd ",
                    };
                    for (const auto& key : keyStrs) {
                        std::string keyStr = bufStr.substr(0, key.size());

                        if (keyStr == "Ka ") {
                            std::stringstream ss(bufStr.substr(key.size()));

                            ss >> ambient.x >> ambient.y >> ambient.z;
                            ambient.w = 1.0f;
                            break;
                        }
                        else if (keyStr == "Kd ") {
                            std::stringstream ss(bufStr.substr(key.size()));

                            ss >> diffuse.x >> diffuse.y >> diffuse.z;
                            diffuse.w = 1.0f;
                            break;
                        }
                        else if (keyStr == "Ks ") {
                            std::stringstream ss(bufStr.substr(key.size()));

                            ss >> specular.x >> specular.y >> specular.z;
                            specular.w = 1.0f;
                            break;
                        }
                        else if (keyStr == "map_Kd ") {
                            std::stringstream ss(bufStr.substr(key.size()));
                            while (std::getline(ss, diffuseTexStr, '\\')) {}

                            ss = std::stringstream(diffuseTexStr);
                            while (std::getline(ss, diffuseTexStr, ' ')) {}
                            break;
                        }
                    }
                }
            }
            if (!materialName.empty()) {

                CMaterialBase* pMaterial = materialManager->CreateMaterial(materialName);
                pMaterial->SetDiffuse(diffuse);
                pMaterial->SetAmbient(ambient);
                pMaterial->SetSpecular(specular);
                pMaterial->SetTexture(diffuseTexStr);
            }
        }

        return true;
    }

} // namespace