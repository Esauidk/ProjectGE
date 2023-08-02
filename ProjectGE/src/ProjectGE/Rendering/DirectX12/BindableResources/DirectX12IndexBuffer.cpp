#include "gepch.h"
#include "DirectX12IndexBuffer.h"

namespace ProjectGE {
	DirectX12IndexBuffer::DirectX12IndexBuffer(WORD* buffer, int count) : DirectX12Buffer<WORD>::DirectX12Buffer(buffer, count, "Index Buffer"), m_Count(count) {}

	void DirectX12IndexBuffer::Bind(ID3D12GraphicsCommandList6* cmdList) {
		DirectX12Context::SyncJob(this->m_UploadSignal, D3D12_COMMAND_LIST_TYPE_COPY);
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			this->m_GpuBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER
		);
		cmdList->ResourceBarrier(1, &barrier);
		D3D12_INDEX_BUFFER_VIEW view;
		view.BufferLocation = this->m_GpuBuffer->GetGPUVirtualAddress();
		view.Format = DXGI_FORMAT_R16_UINT;
		view.SizeInBytes = this->m_BufferSize;
		
		cmdList->IASetIndexBuffer(&view);
	}
	UINT DirectX12IndexBuffer::GetCount()
	{
		return m_Count;
	}
};
