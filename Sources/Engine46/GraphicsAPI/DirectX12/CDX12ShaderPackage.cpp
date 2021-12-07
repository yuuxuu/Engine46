/**
 * @file CDX12ShaderPackage.cpp
 * @brief
 * @author 木村優
 * @date 2021/08/28
 */

#include "CDX12ShaderPackage.h"
#include "CDX12Device.h"
#include "CDX12Command.h"
#include "CDX12Renderer.h"

#include "../CRendererSystem.h"

namespace Engine46 {

    // コンストラクタ
    CDX12ShaderPackage::CDX12ShaderPackage(CDX12Device* pDevice, CDX12Command* pCommand, const char* shaderName) :
        CShaderPackage(shaderName),
        pDX12Device(pDevice),
        pDX12Command(pCommand)
    {}

    // デストラクタ
    CDX12ShaderPackage::~CDX12ShaderPackage()
    {}

    // グラフィクス初期化
    bool CDX12ShaderPackage::InitializeGraphics(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc) {

        CShaderBase* pShader = GetShader(SHADER_TYPE::TYPE_VERTEX);

        if (!pShader) return false;

        ComPtr<ID3DBlob> pBlob;
        HRESULT hr = D3DGetBlobPart(pShader->GetBuffer(), pShader->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &pBlob);
        if (FAILED(hr)) return false;

        if (!pDX12Device->CreateRootSignature(m_pGraphicsRootSignature, pBlob.Get())) return false;

        SetShaderByteCode(gpsDesc);
        SetDepthStencilState(gpsDesc);
        SetRasterizerState(gpsDesc);

        D3D12_INPUT_ELEMENT_DESC ieDesc[] = {
            { "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR"    , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT		, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        gpsDesc.InputLayout = { ieDesc, _countof(ieDesc) };
        gpsDesc.pRootSignature = m_pGraphicsRootSignature.Get();
        gpsDesc.SampleDesc = { 1, 0 };
        gpsDesc.SampleMask = UINT_MAX;

        if (!pDX12Device->CreateGraphicsPipelineState(m_pGraphicsPiplineState, gpsDesc)) return false;

        return true;
    }

    // コンピュート初期化
    bool CDX12ShaderPackage::InitializeCompute(D3D12_COMPUTE_PIPELINE_STATE_DESC& cpuDesc) {

        CShaderBase* pShader = GetShader(SHADER_TYPE::TYPE_COMPUTE);

        if (!pShader) return false;

        ComPtr<ID3DBlob> pBlob;
        HRESULT hr = D3DGetBlobPart(pShader->GetBuffer(), pShader->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &pBlob);
        if (FAILED(hr)) return false;

        D3D12_SHADER_BYTECODE bytecode = {};
        bytecode.pShaderBytecode = pShader->GetBuffer();
        bytecode.BytecodeLength = pShader->GetBufferSize();

        cpuDesc.CS = bytecode;
        cpuDesc.pRootSignature = m_pComputeRootSignature.Get();

        if (!pDX12Device->CreateRootSignature(m_pComputeRootSignature, pBlob.Get())) return false;

        if (!pDX12Device->CreateComputePipelineState(m_pComputePiplineState, cpuDesc)) return false;

        return true;
    }

    // 初期化
    bool CDX12ShaderPackage::Initialize() {

        return true;
    }

    // シェーダーへ設定
    void CDX12ShaderPackage::SetShader() {
        if (pDescriptorHeap) {
            pDX12Command->SetDescriptorHeaps(pDescriptorHeap);
        }

        if (m_pGraphicsRootSignature) {
            pDX12Command->SetGraphicsRootSignature(m_pGraphicsRootSignature.Get());
        }

        if (m_pGraphicsPiplineState) {
            pDX12Command->SetPipelineState(m_pGraphicsPiplineState.Get());
        }

        if (m_pComputeRootSignature) {
            pDX12Command->SetComputeRootSignature(m_pComputeRootSignature.Get());
        }

        if (m_pComputePiplineState) {
            pDX12Command->SetPipelineState(m_pComputePiplineState.Get());
        }
    }

    // シーンコンスタントバッファをシェーダーへ設定
    void CDX12ShaderPackage::SetSceneConstantBufferToShader(UINT startSlot) {
        CDX12Renderer* pRenderer = dynamic_cast<CDX12Renderer*>(CRendererSystem::GetRendererSystem().GetRenderer());
        if (pRenderer) {
            pRenderer->SetSceneConstantBuffers(startSlot);
        }
    }

    void CDX12ShaderPackage::CopyDescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, UINT destIndex) {

        UINT heapSize = pDX12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        D3D12_CPU_DESCRIPTOR_HANDLE destHandle = pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        destHandle.ptr += heapSize * destIndex;

        pDX12Device->CopyDescriptorsSimple(1, destHandle, srcHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    void CDX12ShaderPackage::SetShaderByteCode(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc) {
        for (const auto& pShader : m_pVecShader) {

            D3D12_SHADER_BYTECODE bytecode = {};
            bytecode.pShaderBytecode = pShader->GetBuffer();
            bytecode.BytecodeLength = pShader->GetBufferSize();

            switch (pShader->GetShaderType()) {
            case SHADER_TYPE::TYPE_VERTEX:
                gpsDesc.VS = bytecode;
                break;
            case SHADER_TYPE::TYPE_PIXEL:
                gpsDesc.PS = bytecode;
                break;
            case SHADER_TYPE::TYPE_HULL:
                gpsDesc.HS = bytecode;
                break;
            case SHADER_TYPE::TYPE_DOMAIN:
                gpsDesc.DS = bytecode;
                break;
            case SHADER_TYPE::TYPE_GEOMETRY:
                gpsDesc.GS = bytecode;
                break;
            }
        }
    }

    void CDX12ShaderPackage::SetRTVFormats(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, std::string& shaderName) {

        gpsDesc.NumRenderTargets = RENDER_TARGET_SIZE;

        gpsDesc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;

        if (shaderName == "GBuffer.hlsl") {
            for (UINT i = 0; i < RENDER_TARGET_SIZE; ++i) {
                gpsDesc.RTVFormats[i] = RENDER_TARGET_FORMATS[i];
            }
        }

        std::vector<std::string> shaderNames = {
            "CPUParticle.hlsl",
            "PostEffect_Blur.hlsl",
            "GBuffer_Lighting.hlsl",
            "LuminanceExtraction.hlsl",
            "Model_Line.hlsl",
            "PointSprite.hlsl",
            "ButterFly.hlsl"
        };

        for (const auto& name : shaderNames) {
            if (name == shaderName) {
                gpsDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
                break;
            }
        }

        gpsDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    }

    void CDX12ShaderPackage::SetDepthStencilState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc) {
        gpsDesc.DepthStencilState.DepthEnable = TRUE;
        gpsDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
        gpsDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        gpsDesc.DepthStencilState.StencilEnable = FALSE;
        gpsDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
        gpsDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
        gpsDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        gpsDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        gpsDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        gpsDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
        gpsDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        gpsDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        gpsDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        gpsDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    }

    void CDX12ShaderPackage::SetRasterizerState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc) {
        gpsDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
        gpsDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
        gpsDesc.RasterizerState.FrontCounterClockwise = FALSE;
        gpsDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        gpsDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        gpsDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        gpsDesc.RasterizerState.DepthClipEnable = TRUE;
        gpsDesc.RasterizerState.MultisampleEnable = FALSE;
        gpsDesc.RasterizerState.AntialiasedLineEnable = FALSE;
        gpsDesc.RasterizerState.ForcedSampleCount = 0;
        gpsDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
    }

    void CDX12ShaderPackage::SetBlendState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, std::string& shaderName) {
        gpsDesc.BlendState.AlphaToCoverageEnable = FALSE;
        gpsDesc.BlendState.IndependentBlendEnable = FALSE;

        for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
            gpsDesc.BlendState.RenderTarget[i].BlendEnable = TRUE;
            gpsDesc.BlendState.RenderTarget[i].SrcBlend = D3D12_BLEND_ONE;
            gpsDesc.BlendState.RenderTarget[i].DestBlend = D3D12_BLEND_ZERO;
            gpsDesc.BlendState.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
            gpsDesc.BlendState.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
            gpsDesc.BlendState.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ZERO;
            gpsDesc.BlendState.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
            gpsDesc.BlendState.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
            gpsDesc.BlendState.RenderTarget[i].LogicOpEnable = FALSE;
            gpsDesc.BlendState.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_CLEAR;
        }
    }

    void CDX12ShaderPackage::SetPrimitiveTopologyType(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, std::string& shaderName) {

        std::vector<std::string> shaderNames = {
            "PointSprite.hlsl",
            "ButterFly.hlsl"
        };
        for (const auto& name : shaderNames) {
            if (name == shaderName) {
                gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

                return;
            }
        }

        if ("Model_Line.hlsl" == shaderName) {
            gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
        }
        else
        {
            gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        }
    }

} // namespacce