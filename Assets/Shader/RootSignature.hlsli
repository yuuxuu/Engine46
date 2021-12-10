//****************************************************************************
//!	@file	RootSignature.hlsli
//!	@brief	シェーダーインクルードヘッダー
//!	@note	
//!	@author	2020/03/13 作成：木村優
//*****************************************************************************

#ifndef _ROOT_SIGNATURE_H_
#define _ROOT_SIGNATURE_H_

#define RS_MODEL "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                             "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                             "DescriptorTable(CBV(b1, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                             "DescriptorTable(CBV(b2, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                             "DescriptorTable(CBV(b3, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                             "DescriptorTable(CBV(b4, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                             "DescriptorTable(CBV(b5, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                             "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_PIXEL),"\
                             "StaticSampler(s0,"\
                                 "filter = FILTER_MIN_MAG_MIP_LINEAR,"\
                                 "addressU = TEXTURE_ADDRESS_WRAP,"\
                                 "addressV = TEXTURE_ADDRESS_WRAP,"\
                                 "addressW = TEXTURE_ADDRESS_WRAP,"\
                                 "mipLodBias = 0.0f,"\
                                 "maxAnisotropy = 16,"\
                                 "comparisonFunc = COMPARISON_NEVER,"\
                                 "borderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK,"\
                                 "minLOD = 0.0f,"\
                                 "maxLOD = 3.402823466e+38f,"\
                                 "space = 0,"\
                                 "visibility = SHADER_VISIBILITY_PIXEL),"\
                             "StaticSampler(s1,"\
                                 "filter = FILTER_MIN_MAG_MIP_LINEAR,"\
                                 "addressU = TEXTURE_ADDRESS_BORDER,"\
                                 "addressV = TEXTURE_ADDRESS_BORDER,"\
                                 "addressW = TEXTURE_ADDRESS_BORDER,"\
                                 "mipLodBias = 0.0f,"\
                                 "maxAnisotropy = 1,"\
                                 "comparisonFunc = COMPARISON_LESS_EQUAL,"\
                                 "borderColor = STATIC_BORDER_COLOR_OPAQUE_WHITE,"\
                                 "minLOD = 0.0f,"\
                                 "maxLOD = 3.402823466e+38f,"\
                                 "space = 0,"\
                                 "visibility = SHADER_VISIBILITY_PIXEL),"\
                             "StaticSampler(s2,"\
                                 "filter = FILTER_MIN_MAG_MIP_POINT,"\
                                 "addressU = TEXTURE_ADDRESS_BORDER,"\
                                 "addressV = TEXTURE_ADDRESS_BORDER,"\
                                 "addressW = TEXTURE_ADDRESS_BORDER,"\
                                 "mipLodBias = 0.0f,"\
                                 "maxAnisotropy = 16,"\
                                 "comparisonFunc = COMPARISON_LESS_EQUAL,"\
                                 "borderColor = STATIC_BORDER_COLOR_OPAQUE_WHITE,"\
                                 "minLOD = 0.0f,"\
                                 "maxLOD = 3.402823466e+38f,"\
                                 "space = 0,"\
                                 "visibility = SHADER_VISIBILITY_PIXEL)"\

#define RS_MODEL_LINE "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                                  "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                                  "DescriptorTable(CBV(b1, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                                  "DescriptorTable(CBV(b2, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                                  "DescriptorTable(CBV(b3, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                                  "DescriptorTable(CBV(b4, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                                  "DescriptorTable(CBV(b5, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\

#define RS_GBUFFER_LIGHTING "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                                        "DescriptorTable(CBV(b2, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                                        "DescriptorTable(CBV(b3, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                                        "DescriptorTable(CBV(b4, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                                        "DescriptorTable(CBV(b5, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                                        "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_PIXEL),"\
                                        "DescriptorTable(SRV(t1, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_PIXEL),"\
                                        "DescriptorTable(SRV(t2, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_PIXEL),"\
                                        "DescriptorTable(SRV(t3, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_PIXEL),"\
                                        "StaticSampler(s0,"\
                                            "filter = FILTER_MIN_MAG_MIP_LINEAR,"\
                                            "addressU = TEXTURE_ADDRESS_WRAP,"\
                                            "addressV = TEXTURE_ADDRESS_WRAP,"\
                                            "addressW = TEXTURE_ADDRESS_WRAP,"\
                                            "mipLodBias = 0.0f,"\
                                            "maxAnisotropy = 16,"\
                                            "comparisonFunc = COMPARISON_NEVER,"\
                                            "borderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK,"\
                                            "minLOD = 0.0f,"\
                                            "maxLOD = 3.402823466e+38f,"\
                                            "space = 0,"\
                                            "visibility = SHADER_VISIBILITY_PIXEL)"\

#define RS_BLUR "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                            "DescriptorTable(CBV(b6, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                            "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                            "StaticSampler(s0,"\
                                "filter = FILTER_MIN_MAG_MIP_LINEAR,"\
                                "addressU = TEXTURE_ADDRESS_BORDER,"\
                                "addressV = TEXTURE_ADDRESS_BORDER,"\
                                "addressW = TEXTURE_ADDRESS_BORDER,"\
                                "mipLodBias = 0.0f,"\
                                "maxAnisotropy = 16,"\
                                "comparisonFunc = COMPARISON_NEVER,"\
                                "borderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK,"\
                                "minLOD = 0.0f,"\
                                "maxLOD = 3.402823466e+38f,"\
                                "space = 0,"\
                                "visibility = SHADER_VISIBILITY_PIXEL)"\

#define RS_BLOOM "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                             "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_PIXEL),"\
                             "DescriptorTable(SRV(t13, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_PIXEL),"\
                             "DescriptorTable(SRV(t14, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_PIXEL),"\
                             "DescriptorTable(SRV(t15, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_PIXEL),"\
                             "DescriptorTable(SRV(t16, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_PIXEL),"\
                             "DescriptorTable(SRV(t17, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_PIXEL),"\
                             "DescriptorTable(SRV(t18, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_PIXEL),"\
                             "DescriptorTable(SRV(t19, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_PIXEL),"\
                             "DescriptorTable(SRV(t20, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_PIXEL),"\
                             "StaticSampler(s0,"\
                                 "filter = FILTER_MIN_MAG_MIP_LINEAR,"\
                                 "addressU = TEXTURE_ADDRESS_BORDER,"\
                                 "addressV = TEXTURE_ADDRESS_BORDER,"\
                                 "addressW = TEXTURE_ADDRESS_BORDER,"\
                                 "mipLodBias = 0.0f,"\
                                 "maxAnisotropy = 16,"\
                                 "comparisonFunc = COMPARISON_NEVER,"\
                                 "borderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK,"\
                                 "minLOD = 0.0f,"\
                                 "maxLOD = 3.402823466e+38f,"\
                                 "space = 0,"\
                                 "visibility = SHADER_VISIBILITY_PIXEL)"\

#define RS_LuminanceExtraction "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                                           "DescriptorTable(SRV(t0, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                                           "StaticSampler(s0,"\
                                           "filter = FILTER_MIN_MAG_MIP_LINEAR,"\
                                           "addressU = TEXTURE_ADDRESS_BORDER,"\
                                           "addressV = TEXTURE_ADDRESS_BORDER,"\
                                           "addressW = TEXTURE_ADDRESS_BORDER,"\
                                           "mipLodBias = 0.0f,"\
                                           "maxAnisotropy = 16,"\
                                           "comparisonFunc = COMPARISON_NEVER,"\
                                           "borderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK,"\
                                           "minLOD = 0.0f,"\
                                           "maxLOD = 3.402823466e+38f,"\
                                           "space = 0,"\
                                           "visibility = SHADER_VISIBILITY_PIXEL)"\

#define RS_CS_BLUR "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                               "DescriptorTable(CBV(b6, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                               "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                               "DescriptorTable(UAV(u1, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\

#define RS_CS_LuminanceExtraction "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                                              "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                                              "DescriptorTable(UAV(u1, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\

#define RS_CS_ToneMap "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                                  "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                                  "DescriptorTable(UAV(u1, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\

#define RS_CS_CLEAR_COLOR "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                                      "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\

#define RS_CS_GPUPARTICLE "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                                      "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\
                                      "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0), visibility = SHADER_VISIBILITY_ALL),"\

#endif
