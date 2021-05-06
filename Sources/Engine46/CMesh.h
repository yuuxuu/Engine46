/**
 * @file CMesh.h
 * @brief
 * @author 木村優
 * @date 2021/05/06
 */

#pragma once

#include "math.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	// 前方宣言
	class CDX11Renderer;

	class CMeshBase {
	protected:
		std::vector<VECTOR3>	m_vecVertex;
		std::vector<VECTOR4>	m_vecColor;
		std::vector<VECTOR2>	m_vecUV;
		std::vector<VECTOR3>	m_vecNormal;
		std::vector<VECTOR3>	m_vecBinormal;
		std::vector<VECTOR3>	m_vecTangent;

		std::vector<DWORD>		m_vecIndexes;

		UINT					m_strides;

	public:
		CMeshBase();
		virtual ~CMeshBase();

		virtual void Create() {};
		virtual void Draw() {};

		void AddVertex(const VECTOR3& vertex) { m_vecVertex.emplace_back(vertex); }
		void AddColor(const VECTOR4& color) { m_vecColor.emplace_back(color); }
		void AddUV(const VECTOR2& uv) { m_vecUV.emplace_back(uv); }
		void AddNormal(const VECTOR3& normal) { m_vecNormal.emplace_back(normal); }
		void AddBinormal(const VECTOR3& binormal) { m_vecBinormal.emplace_back(binormal); }
		void AddTangent(const VECTOR3& tangent) { m_vecTangent.emplace_back(tangent); }
	};

	class CDX11Mesh : public CMeshBase {
	private:
		CDX11Renderer*			pRenderer;

		ComPtr<ID3D11Buffer>	m_pVertexBuffer;
		ComPtr<ID3D11Buffer>	m_pIndexBuffer;

	public:
		explicit CDX11Mesh(CDX11Renderer* pRenderer);
		~CDX11Mesh();

		void Create() override;
		void Draw() override;
	};

} // namespace
