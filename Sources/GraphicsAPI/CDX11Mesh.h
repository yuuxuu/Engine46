/**
 * @file CDX11Mesh.h
 * @brief
 * @author 木村優
 * @date 2021/05/22
 */

#pragma once

#ifndef _CDX11_MESH_H_
#define _CDX11_MESH_H_

#include "../Engine46/CMesh.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	// 前方宣言
	class CDX11Device;
	class CDX11DeviceContext;

	class CDX11Mesh : public CMeshBase {
	private:
		CDX11Device*			pDX11Device;
		CDX11DeviceContext*		pDX11DeviceContext;

		ComPtr<ID3D11Buffer>	m_pVertexBuffer;
		ComPtr<ID3D11Buffer>	m_pIndexBuffer;

	public:
		CDX11Mesh(CDX11Device* pDevice, CDX11DeviceContext* pDeviceContext, const char* meshName);
		~CDX11Mesh();

		void Create() override;
		void Draw() override;
	};

} // namespace

#endif