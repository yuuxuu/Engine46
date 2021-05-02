/**
 * @file DX11Renderer.h
 * @brief
 * @author 木村優
 * @date 2021/05/02
 */

#pragma once

#ifndef _CDX11RENDERER_H_
#define _CDX11RENDERER_H_

#include "IRenderer.h"

namespace Engine46 {

	class CDX11Renderer : public IRenderer{
	private:

	public:
		CDX11Renderer();
		~CDX11Renderer();

		bool Initialize() override;
		void Finalize() override;

		bool Render() override;
	};

} // namespace

#endif