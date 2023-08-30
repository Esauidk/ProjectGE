#pragma once
#include "ProjectGE/Core/Log.h"
#include "ProjectGE/Rendering/DirectX12/Util/third-party/d3dx12.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"
#include "ProjectGE/Rendering/DirectX12/Util/DirectX12BarrierTracker.h"
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12Resource.h"


namespace ProjectGE {
	template <typename T>
	class DirectX12Buffer {
	public:
		DirectX12Buffer(T* buffer, UINT count, std::string bufferName = "Personal Buffer") : m_BufferSize(sizeof(T)* count){

			auto& core = DirectX12Core::GetCore();
			auto pDevice = core.GetDevice();
			auto& copyContext = core.GetCopyCommandContext();
			auto& pCommandList = copyContext.GetCommandList();

			// Define heap details
			CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
			CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(m_BufferSize);

			ComPtr<ID3D12Resource2> cpuBuffer;
			ComPtr<ID3D12Resource2> gpuBuffer;
			// Create the GPU and CPU buffers
			bool res = FAILED(pDevice->CreateCommittedResource(
				&defaultHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&bufferDesc,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(&gpuBuffer)
			));

			GE_CORE_ASSERT(!res, "Failed to create GPU Buffer {0}", bufferName);

			auto nameConvert = std::wstring(bufferName.begin(), bufferName.end());
			gpuBuffer->SetName(nameConvert.c_str());

			res = FAILED(pDevice->CreateCommittedResource(
				&uploadHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&bufferDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&cpuBuffer)
			));

			GE_CORE_ASSERT(!res, "Failed to create CPU Buffer {0}", bufferName);
			cpuBuffer->SetName(nameConvert.c_str());

			// Store the data inside
			D3D12_SUBRESOURCE_DATA data = {};
			data.pData = reinterpret_cast<BYTE*>(buffer);
			data.RowPitch = m_BufferSize;
			data.SlicePitch = data.RowPitch;

			UpdateSubresources((&pCommandList), gpuBuffer.Get(), cpuBuffer.Get(), 0, 0, 1, &data);

			m_UploadSignal = copyContext.FinalizeCommandList();

			m_GpuBuffer = std::make_shared<DirectX12Resource>(gpuBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST);
			m_CpuBuffer = std::make_shared<DirectX12Resource>(cpuBuffer.Get(), D3D12_RESOURCE_STATE_GENERIC_READ);
		}

		DirectX12Buffer(UINT count, std::string bufferName = "Personal Buffer") : m_BufferSize(sizeof(T)* count){

			auto& core = DirectX12Core::GetCore();
			auto pDevice = core.GetDevice();

			// Define heap details
			CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
			CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(m_BufferSize);

			ComPtr<ID3D12Resource2> cpuBuffer;
			ComPtr<ID3D12Resource2> gpuBuffer;

			// Create the GPU and CPU buffers
			bool res = FAILED(pDevice->CreateCommittedResource(
				&defaultHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&bufferDesc,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(&gpuBuffer)
			));

			GE_CORE_ASSERT(!res, "Failed to create GPU Buffer {0}", bufferName);

			auto nameConvert = std::wstring(bufferName.begin(), bufferName.end());
			gpuBuffer->SetName(nameConvert.c_str());

			res = FAILED(pDevice->CreateCommittedResource(
				&uploadHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&bufferDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&cpuBuffer)
			));

			GE_CORE_ASSERT(!res, "Failed to create CPU Buffer {0}", bufferName);
			cpuBuffer->SetName(nameConvert.c_str());

			m_GpuBuffer = std::make_shared<DirectX12Resource>(gpuBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST);
			m_CpuBuffer = std::make_shared<DirectX12Resource>(cpuBuffer.Get(), D3D12_RESOURCE_STATE_GENERIC_READ);
		}

		D3D12_GPU_VIRTUAL_ADDRESS GetGPUReference() {
			return m_GpuBuffer->GetResource()->GetGPUVirtualAddress();
		}

		void SetUploadGPUBlock() {
			auto& core = DirectX12Core::GetCore();
			core.InitializeQueueWait(DirectX12QueueType::Copy, DirectX12QueueType::Direct, m_UploadSignal);
		}

		DirectX12BarrierTracker& GetStateTracker() {
			return m_GpuBuffer->GetResourceStateTracker();
		}

		UINT GetSize() const { return m_BufferSize; }

		void UpdateData(void* newData) {
			auto& core = DirectX12Core::GetCore();
			auto& copyContext = core.GetCopyCommandContext();
			auto& pCommandList = copyContext.GetCommandList();
			// Store the data inside
			D3D12_SUBRESOURCE_DATA data = {};
			data.pData = reinterpret_cast<BYTE*>(newData);
			data.RowPitch = m_BufferSize;
			data.SlicePitch = data.RowPitch;

			UpdateSubresources((&pCommandList), m_GpuBuffer->GetResource(), m_CpuBuffer->GetResource(), 0, 0, 1, &data);
			m_UploadSignal = copyContext.FinalizeCommandList();
		}

	private:
		Ref<DirectX12Resource> m_GpuBuffer;
		Ref<DirectX12Resource> m_CpuBuffer;
		UINT m_BufferSize;
		UINT m_UploadSignal;
	};
};
