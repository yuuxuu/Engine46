/**
 * @file CMesh.h
 * @brief
 * @author 木村優
 * @date 2021/05/06
 */

#pragma once

#ifndef _CMESH_H_
#define _CMESH_H_

#include "math.h"

namespace Engine46 {

	struct vertexInfo {
		VECTOR3	vertex;
		VECTOR4	color;
		VECTOR2	uv;
		VECTOR3	normal;
		VECTOR3	tangent;
		VECTOR3	binormal;
	};

	class CMeshBase {
	protected:
		std::vector<vertexInfo>	m_vecVertexInfo;

		std::vector<DWORD>		m_vecIndexes;

		int						m_meshID;

		std::unique_ptr<char[]>	m_meshName;

	public:
		CMeshBase();
		explicit CMeshBase(const char* name);
		virtual ~CMeshBase();

		virtual void Create() {};
		virtual void Draw() {};

		void ReserveVertex(int reserveSize);
		void ReserveIndex(int reserveSize);

		void AddVertexInfo(vertexInfo info) { m_vecVertexInfo.emplace_back(info); }

		void AddIndex(const DWORD index) { m_vecIndexes.emplace_back(index); }

		char* GetMeshName() const { return m_meshName.get(); }
	};

} // namespace

#endif