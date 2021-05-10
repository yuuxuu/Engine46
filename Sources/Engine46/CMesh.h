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

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	// 前方宣言
	class CDX11Renderer;

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

	public:
		CMeshBase();
		virtual ~CMeshBase();

		virtual void Create() {};
		virtual void Draw() {};

		void ReserveVertex(int reserveSize);
		void ReserveIndex(int reserveSize);

		void AddVertexInfo(vertexInfo info) { m_vecVertexInfo.emplace_back(info); }

		void AddIndex(const DWORD index) { m_vecIndexes.emplace_back(index); }
	};

	class CDX11Mesh : public CMeshBase {
	private:
		CDX11Renderer*			pDX11Renderer;

		ComPtr<ID3D11Buffer>	m_pVertexBuffer;
		ComPtr<ID3D11Buffer>	m_pIndexBuffer;

	public:
		explicit CDX11Mesh(CDX11Renderer* pRenderer);
		~CDX11Mesh();

		void Create() override;
		void Draw() override;
	};

} // namespace

#endif