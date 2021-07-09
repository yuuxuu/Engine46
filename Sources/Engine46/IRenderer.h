/**
 * @file IRenderer.h
 * @brief
 * @author 木村優
 * @date 2021/05/02
 */

#pragma once

#ifndef _IRENDERER_H_
#define _IRENDERER_H_

#include "CScene.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	class IRenderer {
	protected:
		IRenderer() {};
		virtual ~IRenderer() {};

		virtual bool Initialize(HWND hwnd, UINT width, UINT height) = 0;
		
		virtual void Finalize() = 0;

		virtual void Begine(CSceneBase* pScene) = 0;

		virtual bool Render(CSceneBase* pScene) = 0;
	};

	enum class RENDERER_TYPE {
		FORWARD,
		DEFERRED,
		TOONMAP,
		POSTEFFECT,
		DYNAMICCUBE,
		MAX,
	};

	enum class SRV_ID {
		DIFFUSE,
		SPECULAR,
		NORMAL,
		POSITION,
		LIGHTMAP,
		DEPTH,
		STENCIL,
		CUBE,
		DYNAMICCUBE,
		ALBEDO,
		DISPLACEMENT,
		ROUGHNESS,
		HDR,
		TOONMAP,
		BLUR,
		MAX = BLUR + 4,
	};

	enum class CB_ID {
		MAIN,
		MATERIAL,
		LIGHT,
		MATRIX,
		DYNAMICCUBE,
		PBR_PARAM,
		BLUR,
		TESS_PARAM,
		WAVE_PARAM,
		PARTICLE_PARAM,
		TONEMAP_PARAM,
		MAX,
	};

	enum class BUFFER_TYPE {
		VERTEX,
		INDEX,
		UNORDERED,
		NONE,
	};

	enum class DIRECTX_TYPE {
		DIRECTX11,
		DIRECTX12,
		NONE,
	};

	enum class RS_TYPE {
		RS_01,
		RS_02,
		RS_03,
		RS_NONE,
	};

	enum class RS01_Param {
		CBV_0,
		CBV_1,
		CBV_2,
		CBV_9,
		SRV_0,
		SRV_1,
		SRV_2,
		SRV_3,
		SRV_4,
		SRV_D0,
		SRV_S0,
		RS01_MAX,
	};

	enum class RS02_Param {
		CBV_9,
		UAV_0,
		RS02_MAX,
	};

	enum class RS03_Param {
		CBV_0,
		CBV_1,
		CBV_3,
		SRV_0,
		RS03_MAX,
	};

} // namespace

#endif