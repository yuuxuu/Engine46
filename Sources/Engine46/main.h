/**
 * @file main.h
 * @brief 
 * @author 木村優
 * @date 2018/12/15
 */

#define _CRT_SECURE_NO_WARNINGS

#pragma once

#ifndef _MAIN_H_
#define _MAIN_H_

using Microsoft::WRL::ComPtr;

constexpr int SCREEN_X = 1280;
constexpr int SCREEN_Y = 720;

constexpr int FULL_SCREEN_X = 1920;
constexpr int FULL_SCREEN_Y = 1080;

void CallConsole();

namespace Engine46 {	

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