/**
 * @file CConstantBuffer.h
 * @brief
 * @author 木村優
 * @date 2021/05/07
 */

#pragma once

#ifndef _CCONSTANTBUFFER_H_
#define _CCONSTANTBUFFER_H_

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	// 前方宣言
	class CDX11Renderer;

	class CCbBase {
	protected:

	public:
		CCbBase();
		virtual ~CCbBase();

		virtual void CreateConstantBuffer(UINT byteWidth) {};
		virtual void Update(void* srcData) {};
		virtual void Set(UINT slot) {};
	};

	class CDX11CB : public CCbBase {
	private:
		CDX11Renderer*			pDX11Renderer;

		ComPtr<ID3D11Buffer>	m_pConstantBuffer;

	public:
		CDX11CB(CDX11Renderer* pRenderer);
		~CDX11CB();

		void CreateConstantBuffer(UINT byteWidth) override;
		void Update(void* srcData) override;
		void Set(UINT slot) override;
	};

} // namespace

#endif
