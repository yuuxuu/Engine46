/**
 * @file CMesh.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/06
 */

#include "CMesh.h"
#include "utility.h"

namespace Engine46 {

	// コンストラクタ
	CMeshBase::CMeshBase() :
		m_meshID(0),
		m_meshName("Mesh_" + std::to_string(m_meshID)),
		m_isInitialize(false)
	{
		m_meshName.resize(m_meshName.size());
	}

	// コンストラクタ
	CMeshBase::CMeshBase(const char* meshName) :
		m_meshID(0),
		m_meshName(meshName),
		m_isInitialize(false)
	{
		m_meshName.resize(m_meshName.size());
	}

	// デストラクタ
	CMeshBase::~CMeshBase()
	{
		VecClear(m_vecVertexInfo);

		VecClear(m_vecIndexes);
	}

	// 頂点配列の初期化
	void CMeshBase::ReserveVertex(int reserveSize) {
		m_vecVertexInfo.reserve(reserveSize);
	}

	// インデックス配列の初期化
	void CMeshBase::ReserveIndex(int reserveSize) {
		m_vecIndexes.reserve(reserveSize);
	}

} // namespace