/**
 * @file CMesh.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/06
 */

#include "CMesh.h"
#include "utility.h"

namespace Engine46 {

	static UINT g_meshCount = 0;

	// コンストラクタ
	CMeshBase::CMeshBase() :
		m_meshID(g_meshCount++)
	{
		std::string str = "mesh_" + std::to_string(g_meshCount);
		int size = (int)str.size() + 1;
		m_meshName.reset(new char[size]);
		str.resize(size);
		str.copy(m_meshName.get(), size);
	}

	// コンストラクタ
	CMeshBase::CMeshBase(const char* name) :
		m_meshID(g_meshCount++)
	{
		std::string str = name;
		int size = (int)str.size() + 1;
		m_meshName.reset(new char[size]);
		str.resize(size);
		str.copy(m_meshName.get(), size);
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