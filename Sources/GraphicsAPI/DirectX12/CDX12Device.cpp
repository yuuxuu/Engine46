/**
 * @file CDX12Device.cpp
 * @brief
 * @author 木村優
 * @date 2021/08/17
 */

#include "CDX12Device.h"

namespace Engine46 {

    constexpr UINT BUFFER_COUNT = 2;

    // コンストラクタ
    CDX12Device::CDX12Device()
    {}

    // デストラクタ
    CDX12Device::~CDX12Device()
    {}

    // 初期化
    bool CDX12Device::Initialize(ComPtr<ID3D12CommandQueue>& pCommandQueue, HWND hwnd, UINT width, UINT height) {

        HRESULT hr;

        UINT flag = 0;
        ComPtr<ID3D12Debug> debug;

#if _DEBUG
        D3D12GetDebugInterface(IID_PPV_ARGS(&debug));

        if (debug) {
            debug->EnableDebugLayer();
        }
        flag |= DXGI_CREATE_FACTORY_DEBUG;
#endif
        ComPtr<IDXGIFactory3> factry;

        hr = CreateDXGIFactory2(flag, IID_PPV_ARGS(&factry));
        if (FAILED(hr)) return false;

        ComPtr<IDXGIAdapter> adapter;

        hr = factry->EnumAdapters(0, &adapter);
        if (FAILED(hr)) return false;

        // D3D12デバイス作成
        hr = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_1, IID_PPV_ARGS(&m_pDevice));
        if (FAILED(hr)) return false;

        D3D12_COMMAND_QUEUE_DESC cqDesc = {};

        cqDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

        if (!CreateCommandQueue(pCommandQueue, cqDesc)) return false;

        DXGI_SWAP_CHAIN_DESC scDesc = {};

        scDesc.BufferCount = BUFFER_COUNT;
        scDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        scDesc.BufferDesc.Width = width;
        scDesc.BufferDesc.Height = height;
        scDesc.BufferDesc.RefreshRate.Numerator = 60;
        scDesc.BufferDesc.RefreshRate.Denominator = 1;
        scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scDesc.OutputWindow = hwnd;
        scDesc.SampleDesc.Count = 1;
        scDesc.SampleDesc.Quality = 0;
        scDesc.Windowed = TRUE;
        scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        // スワップチェーン作成
        hr = factry->CreateSwapChain(pCommandQueue.Get(), &scDesc, (IDXGISwapChain**)m_pSwapChain.GetAddressOf());
        if (FAILED(hr)) return false;

        return true;
    }

    bool CDX12Device::GetBackBuffer(ComPtr<ID3D12Resource>& pResource, UINT index) {

        HRESULT hr = m_pSwapChain->GetBuffer(index, IID_PPV_ARGS(&pResource));
        if (FAILED(hr)) return false;

        return true;
    }

    bool CDX12Device::Present() {

        HRESULT hr = m_pSwapChain->Present(1, 0);
        if (FAILED(hr)) {
            return false;
        }

        return true;
    }

    // フェンス作成
    bool CDX12Device::CreateFence(ComPtr<ID3D12Fence>& pFence) {

        HRESULT hr = m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pFence));
        if (FAILED(hr)) return false;

        return true;
    }

    // コマンドキュー作成
    bool CDX12Device::CreateCommandQueue(ComPtr<ID3D12CommandQueue>& pCommandQueue, D3D12_COMMAND_QUEUE_DESC cqDesc) {

        // コマンドキュー作成
        HRESULT hr = m_pDevice->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&pCommandQueue));
        if (FAILED(hr)) return false;

        return true;
    }

    // コマンドアロケーター作成
    bool CDX12Device::CreateCommandAllcator(ComPtr<ID3D12CommandAllocator>& pCommandAllocator, D3D12_COMMAND_LIST_TYPE type) {

        HRESULT hr = m_pDevice->CreateCommandAllocator(type, IID_PPV_ARGS(&pCommandAllocator));
        if (FAILED(hr)) return false;

        return true;
    }

    // コマンドリスト作成
    bool CDX12Device::CreateCommandList(
        ComPtr<ID3D12GraphicsCommandList>& pCommandList,
        ID3D12CommandAllocator* pCommandAllocator,
        D3D12_COMMAND_LIST_TYPE type) {

        HRESULT hr = m_pDevice->CreateCommandList(0, type, pCommandAllocator, nullptr, IID_PPV_ARGS(&pCommandList));
        if (FAILED(hr)) return false;

        return true;
    }

    // ルートシグネチャ作成
    bool CDX12Device::CreateRootSignature(
        ComPtr<ID3D12RootSignature>& pRootSignature,
        ID3DBlob* pSignatureBlob,
        D3D12_ROOT_SIGNATURE_DESC* pRsDesc) {

        HRESULT hr;

        if (pRsDesc) {
            ComPtr<ID3DBlob> signature;
            ComPtr<ID3DBlob> error;

            hr = D3D12SerializeRootSignature(pRsDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
            if (FAILED(hr)) {
                std::cout << static_cast<BYTE*>(error->GetBufferPointer()) << std::endl;
                return false;
            }

            hr = m_pDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&pRootSignature));
            if (FAILED(hr)) return false;
        }

        if (pSignatureBlob) {
            hr = m_pDevice->CreateRootSignature(0, pSignatureBlob->GetBufferPointer(), pSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&pRootSignature));
            if (FAILED(hr)) return false;
        }

        return true;
    }

    // グラフィクスパイプラインステート作成
    bool CDX12Device::CreateGraphicsPipelineState(ComPtr<ID3D12PipelineState>& pPipelineState, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc) {

        HRESULT hr = m_pDevice->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&pPipelineState));
        if (FAILED(hr)) return false;

        return true;
    }

    // コンピュートパイプラインステート作成
    bool CDX12Device::CreateComputePipelineState(ComPtr<ID3D12PipelineState>& pPipelineState, D3D12_COMPUTE_PIPELINE_STATE_DESC& cpsDesc) {

        HRESULT hr = m_pDevice->CreateComputePipelineState(&cpsDesc, IID_PPV_ARGS(&pPipelineState));
        if (FAILED(hr)) return false;

        return true;
    }

    // デスクリプターヒープ作成
    bool CDX12Device::CreateDescriptorHeap(ComPtr<ID3D12DescriptorHeap>& pDescriptorHeap, D3D12_DESCRIPTOR_HEAP_DESC& dhDesc) {

        HRESULT hr = m_pDevice->CreateDescriptorHeap(&dhDesc, IID_PPV_ARGS(&pDescriptorHeap));
        if (FAILED(hr)) return false;

        return true;
    }

    // リソース作成
    bool CDX12Device::CreateResource(
        ComPtr<ID3D12Resource>& pResource,
        D3D12_HEAP_PROPERTIES& prop,
        D3D12_RESOURCE_DESC& rDesc,
        D3D12_CLEAR_VALUE* clearValue,
        D3D12_RESOURCE_STATES states) {

        HRESULT hr = m_pDevice->CreateCommittedResource(
            &prop,
            D3D12_HEAP_FLAG_NONE,
            &rDesc,
            states,
            clearValue,
            IID_PPV_ARGS(&pResource)
        );
        if (FAILED(hr)) return false;

        return true;
    }

    // レンダーターゲットビュー作成
    void CDX12Device::CreateRenderTargetView(
        ID3D12Resource* pResource,
        D3D12_RENDER_TARGET_VIEW_DESC* pRtvDesc,
        D3D12_CPU_DESCRIPTOR_HANDLE handle) {

        m_pDevice->CreateRenderTargetView(pResource, pRtvDesc, handle);
    }

    // デプスステンシルビュー作成
    void CDX12Device::CreateDepthStencilView(
        ID3D12Resource* pResource,
        D3D12_DEPTH_STENCIL_VIEW_DESC* pDsvDesc,
        D3D12_CPU_DESCRIPTOR_HANDLE handle) {

        m_pDevice->CreateDepthStencilView(pResource, pDsvDesc, handle);
    }

    // コンスタントバッファビュー作成
    void CDX12Device::CreateConstantBufferView(
        ID3D12Resource* pResource,
        D3D12_CONSTANT_BUFFER_VIEW_DESC& cbvDesc,
        D3D12_CPU_DESCRIPTOR_HANDLE handle) {

        m_pDevice->CreateConstantBufferView(&cbvDesc, handle);
    }

    // シェーダーリソースビュー作成
    void CDX12Device::CreateShaderResourceView(
        ID3D12Resource* pResource,
        D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc,
        D3D12_CPU_DESCRIPTOR_HANDLE handle) {

        m_pDevice->CreateShaderResourceView(pResource, &srvDesc, handle);
    }

    // アンオーダードアクセスビュー作成
    void CDX12Device::CreateUnorderedAccessView(
        ID3D12Resource* pResource,
        D3D12_UNORDERED_ACCESS_VIEW_DESC& uavDesc,
        D3D12_CPU_DESCRIPTOR_HANDLE handle) {

        m_pDevice->CreateUnorderedAccessView(pResource, nullptr, &uavDesc, handle);
    }

    // サンプラー作成
    void CDX12Device::CreateSampler(D3D12_CPU_DESCRIPTOR_HANDLE handle, D3D12_SAMPLER_DESC& sDesc) {

        //sDesc.Filter			= D3D12_FILTER_MIN_MAG_MIP_LINEAR;
        //sDesc.AddressU			= D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        //sDesc.AddressV			= D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        //sDesc.AddressW			= D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        //sDesc.BorderColor[0]	= 1.0f;
        //sDesc.BorderColor[1]	= 1.0f;
        //sDesc.BorderColor[2]	= 1.0f;
        //sDesc.BorderColor[3]	= 1.0f;
        //sDesc.ComparisonFunc	= D3D12_COMPARISON_FUNC_NEVER;
        //sDesc.MaxAnisotropy		= 1;
        //sDesc.MipLODBias		= 0;
        //sDesc.MinLOD			= -FLT_MAX;
        //sDesc.MaxLOD			=  FLT_MAX;

        m_pDevice->CreateSampler(&sDesc, handle);
    }

    void CDX12Device::CopyDescriptorsSimple(
        UINT numDescriptors,
        D3D12_CPU_DESCRIPTOR_HANDLE destHandle,
        D3D12_CPU_DESCRIPTOR_HANDLE srcHandle,
        D3D12_DESCRIPTOR_HEAP_TYPE heapType) {

        m_pDevice->CopyDescriptorsSimple(
            1,
            destHandle,
            srcHandle,
            heapType);
    }

} // namespace
