/**
 * @file CDX12Texture.cpp
 * @brief
 * @author 木村優
 * @date 2021/08/27
 */

#include "CDX12Texture.h"
#include "CDX12Device.h"
#include "CDX12Command.h"

#include "CDX12ShaderPackage.h"

namespace Engine46 {

	// コンストラクタ
	CDX12Texture::CDX12Texture(CDX12Device* pDevice, CDX12Command* pCommand, const char* textureName):
		CTextureBase(textureName),
		pDX12Device(pDevice),
		pDX12Command(pCommand)
	{}

	// デストラクタ
	CDX12Texture::~CDX12Texture()
	{}

	// テクスチャ作成
	void CDX12Texture::CreateTexture() {

		pDX12Command->ResetCommandList();

		D3D12_RESOURCE_DESC rDesc = {};

		rDesc.Width				= m_textureData.width;
		rDesc.Height			= m_textureData.height;
		rDesc.MipLevels			= 1;
		rDesc.Format			= m_textureData.format;
		rDesc.Flags				= D3D12_RESOURCE_FLAG_NONE;
		rDesc.DepthOrArraySize	= 1;
		rDesc.SampleDesc		= { 1 , 0 };
		rDesc.Dimension			= D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		rDesc.Layout			= D3D12_TEXTURE_LAYOUT_UNKNOWN;

		CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		pDX12Device->CreateResource(m_pTextureResource, prop, rDesc, nullptr, D3D12_RESOURCE_STATE_COPY_DEST);

		UINT64 bufSize = GetRequiredIntermediateSize(m_pTextureResource.Get(), 0, 1);
		CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufSize);
		prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		
		ComPtr<ID3D12Resource> resource;
		pDX12Device->CreateResource(resource, prop, desc);

		if (m_textureData.pData) {
			D3D12_SUBRESOURCE_DATA subData = {};

			subData.pData		= m_textureData.pData.get();
			subData.RowPitch	= m_textureData.rowPitch;
			subData.SlicePitch	= m_textureData.slicePitch;
			
			UpdateSubresources(
				pDX12Command->GetCommandList(),
				m_pTextureResource.Get(),
				resource.Get(),
				0,
				0,
				1,
				&subData);

			CD3DX12_RESOURCE_BARRIER rBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
				m_pTextureResource.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

			pDX12Command->GetCommandList()->ResourceBarrier(1, &rBarrier);

			pDX12Command->ExecuteComandLists();
		}
	}

	void CDX12Texture::CreateShaderResourceView() {

		D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};

		dhDesc.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		dhDesc.NumDescriptors	= 1;
		dhDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		pDX12Device->CreateDescriptorHeap(m_SrvDescriptorHeap, dhDesc);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format					= m_textureData.format;
		srvDesc.ViewDimension			= D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels		= 1;

		pDX12Device->CreateShaderResourceView(m_pTextureResource.Get(), srvDesc, m_SrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}

	// テクスチャをシェーダーへ設定
	void CDX12Texture::Set(UINT slot) {

		if (m_gpuHandle.ptr != 0) {
			pDX12Command->SetRootDescriptorTable(slot + (UINT)MyRootSignature_01::SRV_diffuse, m_gpuHandle);
			return;
		}

		if (m_SrvDescriptorHeap) {
			//pDX12Command->SetDescriptorHeaps(m_SrvDescriptorHeap.Get());
			//
			//pDX12Command->SetConstantBuffers(slot, m_SrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
		}
	}

	// テクスチャ作成
	void CDX12Texture::CreateTexture(D3D12_RESOURCE_DESC& rDesc, D3D12_CLEAR_VALUE clearValue) {

		CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		pDX12Device->CreateResource(m_pTextureResource, prop, rDesc, &clearValue);

		m_textureData.format = rDesc.Format;
	}

	// シェーダーリソースビュー作成
	void CDX12Texture::CreateShaderResourceView(ID3D12DescriptorHeap* pDescriptorHeap, UINT heapIndex) {
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format					= m_textureData.format;
		srvDesc.ViewDimension			= D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels		= 1;

		UINT heapSize = pDX12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		m_cpuHandle = pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		m_cpuHandle.ptr += heapSize * heapIndex;

		m_gpuHandle = pDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
		m_gpuHandle.ptr += (UINT64)heapSize * heapIndex;

		pDX12Device->CreateShaderResourceView(m_pTextureResource.Get(), srvDesc, m_cpuHandle);
	}

} // namespace
