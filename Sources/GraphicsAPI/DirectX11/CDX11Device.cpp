/**
 * @file CDX11Device.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/22
 */

#include "CDX11Device.h"

namespace Engine46 {

    // コンストラクタ
    CDX11Device::CDX11Device()
    {}

    // デストラクタ
    CDX11Device::~CDX11Device()
    {}

    // 初期化
    bool CDX11Device::Initialize(ComPtr<ID3D11DeviceContext>& m_pDeviceContext, HWND hwnd, UINT width, UINT height) {

        UINT flag = 0;
#ifdef _DEBUG
        flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        // スワップチェーンの設定
        DXGI_SWAP_CHAIN_DESC scDesc = {};
        scDesc.BufferDesc.Width = width;
        scDesc.BufferDesc.Height = height;
        scDesc.BufferDesc.RefreshRate.Numerator = 60;
        scDesc.BufferDesc.RefreshRate.Denominator = 1;
        scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scDesc.SampleDesc.Count = 1;
        scDesc.SampleDesc.Quality = 0;
        scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scDesc.BufferCount = 1;
        scDesc.OutputWindow = hwnd;
        scDesc.Windowed = TRUE;
        scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_10_1,
        };
        UINT numFeatureLevel = sizeof(featureLevels) / sizeof(featureLevels[0]);
        D3D_FEATURE_LEVEL featureLevel;

        // デバイスとスワップチェーン作成
        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            NULL,
            D3D_DRIVER_TYPE_HARDWARE,
            NULL,
            flag,
            featureLevels,
            numFeatureLevel,
            D3D11_SDK_VERSION,
            &scDesc,
            &m_pSwapChain,
            &m_pDevice,
            &featureLevel,
            &m_pDeviceContext);
        if (FAILED(hr)) {
            MessageBox(NULL, "DirectX11Device初期化：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    bool CDX11Device::GetBackBuffer(ID3D11Texture2D*& pTex2D) {

        HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pTex2D);
        if (FAILED(hr)) {
            MessageBox(NULL, "バックバッファ取得：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    bool CDX11Device::Present() {

        HRESULT hr = m_pSwapChain->Present(0, 0);
        if (FAILED(hr)) {
            return false;
        }

        return true;
    }

    // バッファ作成
    bool CDX11Device::CreateBuffer(
        ComPtr<ID3D11Buffer>& pBuffer,
        D3D11_BUFFER_DESC& bufDesc,
        D3D11_SUBRESOURCE_DATA* pInitData) {

        HRESULT hr = m_pDevice->CreateBuffer(&bufDesc, pInitData, &pBuffer);
        if (FAILED(hr)) {
            MessageBox(NULL, "Buffer作成：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    // テクスチャ2D作成
    bool CDX11Device::CreateTexture2D(
        ComPtr<ID3D11Texture2D>& pTex2D,
        D3D11_TEXTURE2D_DESC& texDesc,
        D3D11_SUBRESOURCE_DATA* pInitData) {

        HRESULT hr = m_pDevice->CreateTexture2D(&texDesc, pInitData, &pTex2D);
        if (FAILED(hr)) {
            MessageBox(NULL, "Texture2D作成：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    // レンダーターゲットビュー作成
    bool CDX11Device::CreateRenderTargetView(
        ComPtr<ID3D11RenderTargetView>& pRtv,
        ID3D11Texture2D* pTex2D,
        D3D11_RENDER_TARGET_VIEW_DESC& rtvDesc) {

        HRESULT hr = m_pDevice->CreateRenderTargetView(pTex2D, &rtvDesc, &pRtv);
        if (FAILED(hr)) {
            MessageBox(NULL, "レンダーターゲットビュー作成：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    // シェーダーリソースビュー作成
    bool CDX11Device::CreateShaderResourceView(
        ComPtr<ID3D11ShaderResourceView>& pSrv,
        ID3D11Texture2D* pTex2D,
        D3D11_SHADER_RESOURCE_VIEW_DESC& srvDesc) {

        HRESULT hr = m_pDevice->CreateShaderResourceView(pTex2D, &srvDesc, &pSrv);
        if (FAILED(hr)) {
            MessageBox(NULL, "シェーダーリソースビュー作成：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    // デプスステンシルビュー作成
    bool CDX11Device::CreateDepthStencilView(
        ComPtr<ID3D11DepthStencilView>& pDsv,
        ID3D11Texture2D* pTex2D,
        D3D11_DEPTH_STENCIL_VIEW_DESC& dsvDesc) {

        HRESULT hr = m_pDevice->CreateDepthStencilView(pTex2D, &dsvDesc, &pDsv);
        if (FAILED(hr)) {
            MessageBox(NULL, "デプスステンシルビュー作成：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    // インプットレイアウトを設定
    bool CDX11Device::CreateInputLayout(ComPtr<ID3D11InputLayout>& pILayout, void* pBuf, size_t bufSize) {

        D3D11_INPUT_ELEMENT_DESC elemDesc[] = {
                { "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT   , 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT      , 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "BINORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "BONEINDEX",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "BONEWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 88, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        HRESULT hr = m_pDevice->CreateInputLayout(elemDesc, ARRAYSIZE(elemDesc), pBuf, bufSize, &pILayout);
        if (FAILED(hr)) {
            MessageBox(NULL, "インプットレイアウト作成：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    // サンプラーステートを設定
    bool CDX11Device::CreateSamplerState(ComPtr<ID3D11SamplerState>& pSstate, D3D11_SAMPLER_DESC& sDesc) {

        HRESULT hr = m_pDevice->CreateSamplerState(&sDesc, &pSstate);
        if (FAILED(hr)) {
            MessageBox(NULL, "サンプラーステート作成：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    // ブレンドステートを設定
    bool CDX11Device::CreateBlendState(ComPtr<ID3D11BlendState>& pBState, D3D11_BLEND_DESC& bDesc) {

        HRESULT hr = m_pDevice->CreateBlendState(&bDesc, &pBState);
        if (FAILED(hr)) {
            MessageBox(NULL, "ブレンドステート作成：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    // ラスタライズステートを設定
    bool CDX11Device::CreateRasterizerState(ComPtr<ID3D11RasterizerState>& pRSstate, D3D11_RASTERIZER_DESC& rsDesc) {

        HRESULT hr = m_pDevice->CreateRasterizerState(&rsDesc, &pRSstate);
        if (FAILED(hr)) {
            MessageBox(NULL, "ラスタライザステート作成：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    // デプスステンシルステートを設定
    bool CDX11Device::CreateDepthStencilState(ComPtr<ID3D11DepthStencilState>& pDSstate, D3D11_DEPTH_STENCIL_DESC& dsDesc) {

        HRESULT hr = m_pDevice->CreateDepthStencilState(&dsDesc, &pDSstate);
        if (FAILED(hr)) {
            MessageBox(NULL, "デプスステンシルステート作成：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    // バーテックスシェーダー作成
    bool CDX11Device::CreateVertexShader(ComPtr<ID3D11VertexShader>& pVS, ID3DBlob* pBlob) {

        HRESULT hr = m_pDevice->CreateVertexShader(
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            nullptr,
            &pVS);

        if (FAILED(hr)) {
            MessageBox(NULL, "バーテックスシェーダー作成：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    // ピクセルシェーダー作成
    bool CDX11Device::CreatePixelShader(ComPtr<ID3D11PixelShader>& pPS, ID3DBlob* pBlob) {

        HRESULT hr = m_pDevice->CreatePixelShader(
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            nullptr,
            &pPS);

        if (FAILED(hr)) {
            MessageBox(NULL, "ピクセルシェーダー作成：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    // ハルシェーダー作成
    bool CDX11Device::CreateHullShader(ComPtr<ID3D11HullShader>& pHS, ID3DBlob* pBlob) {

        HRESULT hr = m_pDevice->CreateHullShader(
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            nullptr,
            &pHS);

        if (FAILED(hr)) {
            MessageBox(NULL, "ハルシェーダー作成：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    // ドメインシェーダー作成
    bool CDX11Device::CreateDomainShader(ComPtr<ID3D11DomainShader>& pDS, ID3DBlob* pBlob) {

        HRESULT hr = m_pDevice->CreateDomainShader(
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            nullptr,
            &pDS);

        if (FAILED(hr)) {
            MessageBox(NULL, "ドメインシェーダー作成：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    // ジオメトリーシェーダー作成
    bool CDX11Device::CreateGeometryShader(ComPtr<ID3D11GeometryShader>& pGS, ID3DBlob* pBlob) {

        HRESULT hr = m_pDevice->CreateGeometryShader(
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            nullptr,
            &pGS);

        if (FAILED(hr)) {
            MessageBox(NULL, "ジオメトリーシェーダー作成：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    // コンピュートシェーダー作成
    bool CDX11Device::CreateComputeShader(ComPtr<ID3D11ComputeShader>& pCS, ID3DBlob* pBlob) {

        HRESULT hr = m_pDevice->CreateComputeShader(
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            nullptr,
            &pCS);

        if (FAILED(hr)) {
            MessageBox(NULL, "コンピュートシェーダー作成：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

} // namespace
