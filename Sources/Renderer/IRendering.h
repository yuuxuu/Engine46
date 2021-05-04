/**
 * @file IRendering.h
 * @brief
 * @author 木村優
 * @date 2021/05/04
 */

#pragma once

#ifndef _IRENDERING_H_
#define _IRENDERING_H_

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	class IRendering {
	protected:
		IRendering() {}
		virtual ~IRendering() {}

		virtual bool Initialize(UINT width, UINT height) = 0;
		virtual void Begine() = 0;
		virtual void End() = 0;
	};

} // namespace

#endif