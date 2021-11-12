/**
 * @file CDX12Command.cpp
 * @brief
 * @author 木村優
 * @date 2021/08/18
 */

#include "CDX12Command.h"
#include "CDX12Device.h"

namespace Engine46 {

    // コンストラクタ
    CDX12Command::CDX12Command(ComPtr<ID3D12CommandQueue>& pCommandQueue)
        : m_pCommandQueue(pCommandQueue)
    {}

    // デストラクタ
    CDX12Command::~CDX12Command()
    {}

    // 初期化
    bool CDX12Command::Initialize(CDX12Device* pDevice) {

        if (!pDevice->CreateCommandAllcator(m_pCommandAllocator, D3D12_COMMAND_LIST_TYPE_DIRECT)) return false;

        if (!pDevice->CreateCommandList(m_pCommandList, m_pCommandAllocator.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT)) return false;

        m_fence_event = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);

        if (!pDevice->CreateFence(m_pFence)) return false;

        return true;
    }

    // コマンドリストをクローズ
    bool CDX12Command::CloseCommandList() {

        HRESULT hr = m_pCommandList->Close();
        if (FAILED(hr)) return false;

        return true;
    }

    // コマンドリストをリセット
    bool CDX12Command::ResetCommandList() {

        HRESULT hr = m_pCommandAllocator->Reset();
        if (FAILED(hr)) return false;

        hr = m_pCommandList->Reset(m_pCommandAllocator.Get(), nullptr);
        if (FAILED(hr)) return false;

        return true;
    }

    // コマンドリストを発行
    void CDX12Command::ExecuteComandLists() {

        if (!CloseCommandList()) return;

        ID3D12CommandList* pCommandList = m_pCommandList.Get();

        m_pCommandQueue->ExecuteCommandLists(1, &pCommandList);

        WaitForPreviousFrame(m_pCommandQueue.Get());
    }

    // 前のフレーム待機
    void CDX12Command::WaitForPreviousFrame(ID3D12CommandQueue* pCommandQueue) {
        HRESULT hr;

        ++m_frames;
        const UINT64 fence = m_frames;

        hr = pCommandQueue->Signal(m_pFence.Get(), fence);
        if (FAILED(hr)) return;

        if (m_pFence->GetCompletedValue() < fence) {
            hr = m_pFence->SetEventOnCompletion(fence, m_fence_event);
            if (FAILED(hr)) return;

            WaitForSingleObject(m_fence_event, INFINITE);
        }
    }

    // レンダーターゲットビューをクリア
    void CDX12Command::ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE handle) {
        float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        m_pCommandList->ClearRenderTargetView(handle, color, 0, nullptr);
    }

    // デプスステンシルビューをクリア
    void CDX12Command::ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE handle, D3D12_CLEAR_FLAGS clearFlags) {
        m_pCommandList->ClearDepthStencilView(handle, clearFlags, 1.0f, 0, 0, nullptr);
    }

    // バッファを設定
    void CDX12Command::SetBuffer(D3D12_VERTEX_BUFFER_VIEW& vbView, D3D12_INDEX_BUFFER_VIEW& ibView) {
        m_pCommandList->IASetVertexBuffers(0, 1, &vbView);

        m_pCommandList->IASetIndexBuffer(&ibView);
    }

    // インデックス描画
    void CDX12Command::DrawIndexed(D3D12_PRIMITIVE_TOPOLOGY topology, UINT numIndexes) {
        m_pCommandList->IASetPrimitiveTopology(topology);

        m_pCommandList->DrawIndexedInstanced(numIndexes, 1, 0, 0, 0);
    }

    // Rectを設定
    void CDX12Command::SetRect(UINT width, UINT height) {
        D3D12_RECT rect = { 0, 0, (long)width, (long)height };

        m_pCommandList->RSSetScissorRects(1, &rect);
    }

    // ビューポートを設定
    void CDX12Command::SetViewPort(UINT x, UINT y, UINT width, UINT height) {
        D3D12_VIEWPORT vp = {};

        vp.TopLeftX = (float)x;
        vp.TopLeftY = (float)y;
        vp.Width = (float)width;
        vp.Height = (float)height;
        vp.MinDepth = 0;
        vp.MaxDepth = 1;

        m_pCommandList->RSSetViewports(1, &vp);
    }

    // レンダーターゲットビューを設定
    void CDX12Command::SetRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE* dsHandle) {
        if (rtvHandle) {
            m_pCommandList->OMSetRenderTargets(1, rtvHandle, TRUE, dsHandle);
        }
        else {
            m_pCommandList->OMSetRenderTargets(0, rtvHandle, TRUE, dsHandle);
        }
    }

    // レンダーターゲットビューを設定
    void CDX12Command::SetRenderTargetViews(std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& rtvHandles, D3D12_CPU_DESCRIPTOR_HANDLE* dsHandle) {
        if (!rtvHandles.empty()) {
            m_pCommandList->OMSetRenderTargets((UINT)rtvHandles.size(), &rtvHandles[0], TRUE, dsHandle);
        }
        else {
            m_pCommandList->OMSetRenderTargets(0, nullptr, TRUE, dsHandle);
        }
    }

    // リソースバリアを設定
    void CDX12Command::SetResourceBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) {
        D3D12_RESOURCE_BARRIER rb = {};

        rb.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        rb.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        rb.Transition.pResource = pResource;
        rb.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        rb.Transition.StateBefore = before;
        rb.Transition.StateAfter = after;

        m_pCommandList->ResourceBarrier(1, &rb);
    }

    // デスクリプターヒープを設定
    void CDX12Command::SetDescriptorHeaps(ID3D12DescriptorHeap* pDescriptorHeap) {
        if (pDescriptorHeap) {
            m_pCommandList->SetDescriptorHeaps(1, &pDescriptorHeap);
        }
        else {
            m_pCommandList->SetDescriptorHeaps(0, &pDescriptorHeap);
        }
    }

    // グラフィクスルートシグネチャを設定
    void CDX12Command::SetGraphicsRootSignature(ID3D12RootSignature* pRootSignature) {
        m_pCommandList->SetGraphicsRootSignature(pRootSignature);
    }

    // コンピュートルートシグネチャを設定
    void CDX12Command::SetComputeRootSignature(ID3D12RootSignature* pRootSignature) {
        m_pCommandList->SetComputeRootSignature(pRootSignature);
    }

    // パイプラインステートを設定
    void CDX12Command::SetPipelineState(ID3D12PipelineState* pPipelineState) {
        m_pCommandList->SetPipelineState(pPipelineState);
    }

    // グラフィクスルートデスクリプターヒープテーブルを設定
    void CDX12Command::SetGraphicsRootDescriptorTable(UINT index, D3D12_GPU_DESCRIPTOR_HANDLE handle) {
        m_pCommandList->SetGraphicsRootDescriptorTable(index, handle);
    }

    // コンピュートルートデスクリプターヒープテーブルを設定
    void CDX12Command::SetComputeRootDescriptorTable(UINT index, D3D12_GPU_DESCRIPTOR_HANDLE handle) {
        m_pCommandList->SetComputeRootDescriptorTable(index, handle);
    }

    // グラフィクスコンスタントバッファビューを設定
    void CDX12Command::SetGraphicsRootConstantBufferView(UINT index, D3D12_GPU_VIRTUAL_ADDRESS address) {
        m_pCommandList->SetGraphicsRootConstantBufferView(index, address);
    }

    // グラフィクスシェーダーリソースビューを設定
    void CDX12Command::SetGraphicsRootShaderResourceView(UINT index, D3D12_GPU_VIRTUAL_ADDRESS address) {
        m_pCommandList->SetGraphicsRootShaderResourceView(index, address);
    }

    // グラフィクスアンオーダードアクセスビューを設定
    void CDX12Command::SetGraphicsRootUnorderdAccessView(UINT index, D3D12_GPU_VIRTUAL_ADDRESS address) {
        m_pCommandList->SetGraphicsRootUnorderedAccessView(index, address);
    }

    // ディスパッチ
    void CDX12Command::Dispatch(UINT dispatchX, UINT dispatchY, UINT dispatchZ) {
        m_pCommandList->Dispatch(dispatchX, dispatchY, dispatchZ);

        WaitForPreviousFrame(m_pCommandQueue.Get());
    }

} // namespace
