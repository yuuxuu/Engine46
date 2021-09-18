/**
 * @file CRendering.h
 * @brief
 * @author 木村優
 * @date 2021/05/23
 */

#pragma once

#ifndef _CRENDERING_H_
#define _CRENDERING_H_

#include "CTexture.h"

namespace Engine46 {

	// 前方宣言
	class CConstantBufferBase;

	class CRenderingBase {
	protected:
		std::unique_ptr<CTextureBase> m_pRenderTex;

	public:
		CRenderingBase();
		virtual ~CRenderingBase();

		virtual bool Initialize(UINT width, UINT height) { return true; }
		virtual void Begine() {};
		virtual void End() {};

		CTextureBase* GetRenderTexture() const { return m_pRenderTex.get(); }
	};

} // namespace

#endif