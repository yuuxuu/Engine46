/**
 * @file CDX12ForwardRendering.h
 * @brief
 * @author 木村優
 * @date 2021/09/16
 */

#pragma once

#ifndef _CDX12_FORWARD_RENDERING_H_
#define _CDX12_FORWARD_RENDERING_H_

#include "../Engine46/CRendering.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	// 前方宣言
	class CDX12Device;
	class CDX12Command;
	class CDX12Texture;

	class CDX12ForwardRendering : public CRenderingBase {
	private:
		CDX12Device*					pDX12Device;
		CDX12Command*					pDX12Command;

		CDX12Texture*					pDX12RenderTexture;

		ComPtr<ID3D12DescriptorHeap>	m_pRtvDescriptorHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE		m_rtvHandle;

		ComPtr<ID3D12Resource>			m_pDsvResource;
		ComPtr<ID3D12DescriptorHeap>	m_pDsvDescriptorHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE		m_dsvHandle;

	public:
		CDX12ForwardRendering(CDX12Device* pDX12Device, CDX12Command* pCommand);
		~CDX12ForwardRendering();

		bool Initialize(UINT width, UINT height) override;
		void Begine() override;
		void End() override;

		void Rendering(CSceneBase* pScene) override;
		void DrawForRenderScene(CSprite* pSprite, UINT x, UINT y, UINT width, UINT height) override;
	};

} // namespace

#endif