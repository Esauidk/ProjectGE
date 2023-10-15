#include "gepch.h"
#include "DirectX12RendererAPI.h"
#include "ProjectGE/Rendering/DirectX12/Util/DirectX12Util.h"

namespace ProjectGE {
	DirectX12RendererAPI::DirectX12RendererAPI(): m_Core(DirectX12Core::CreateCore())
	{
	}

	void DirectX12RendererAPI::SetClearColor(const glm::vec4& color)
	{
	}

	void DirectX12RendererAPI::Clear()
	{
	}

	void DirectX12RendererAPI::DrawIndexed(UINT indecies, UINT instances)
	{
		
		auto& core = ProjectGE::DirectX12Core::GetCore();
		auto& context = core.GetDirectCommandContext();
		auto& list = context.GetCommandList();
		auto& state = context.GetStateManager();

		state.BindState();
		list->DrawIndexedInstanced(indecies, instances, 0, 0, 0);
	}

	Ref<VertexBuffer> DirectX12RendererAPI::CreateVertexBuffer(BYTE* data, int vertexByteSize, int vertexCount)
	{
		return Ref<VertexBuffer>(new DirectX12VertexBuffer(data, vertexByteSize, vertexCount));
	}

	void DirectX12RendererAPI::SetVertexBuffer(Ref<VertexBuffer> vbuffer)
	{
		auto& context = m_Core.GetDirectCommandContext();
		auto dx12Buf = CastPtr<DirectX12VertexBuffer>(vbuffer);
		D3D12_VERTEX_BUFFER_VIEW view = dx12Buf->GenerateView();
		context.GetStateManager().SetVBV(view);
	}

	Ref<IndexBuffer> DirectX12RendererAPI::CreateIndexBuffer(unsigned int* data, unsigned int indexCount)
	{
		return Ref<IndexBuffer>(new DirectX12IndexBuffer(data, indexCount));
	}

	void DirectX12RendererAPI::SetIndexBuffer(Ref<IndexBuffer> ibuffer)
	{
		auto& context = m_Core.GetDirectCommandContext();
		auto dx12Buf = CastPtr<DirectX12IndexBuffer>(ibuffer);
		D3D12_INDEX_BUFFER_VIEW view = dx12Buf->GenerateView();
		context.GetStateManager().SetIBV(view);
	}

	Ref<Shader> DirectX12RendererAPI::LoadShader(std::string path)
	{
		return Ref<Shader>(new DirectX12Shader(path));
	}

	Ref<ConstantBuffer> DirectX12RendererAPI::CreateConstantBuffer(UINT size)
	{
		return Ref<ConstantBuffer>(new DirectX12ConstantBuffer(size));
	}

	void DirectX12RendererAPI::SetConstantBuffer(Ref<ConstantBuffer> cbuffer, Stages stage, ShaderConstantType constantType)
	{
		auto& context = m_Core.GetDirectCommandContext();
		auto dx12Buf = CastPtr<DirectX12ConstantBuffer>(cbuffer);
		D3D12_GPU_VIRTUAL_ADDRESS add = dx12Buf->GetGPUReference();
		D3D12_CPU_DESCRIPTOR_HANDLE descriptor = dx12Buf->GetDescriptor();

		DirectX12ConstantBufferView view;
		view.m_View = descriptor;
		view.m_GPUAdd = add;

		UINT index;

		if (constantType == ShaderConstantType::Global) {
			index = 0;
		}
		else {
			index = 1;
		}

		context.GetStateManager().SetCBV(stage, view, index);
	}

	Ref<Texture2D> DirectX12RendererAPI::CreateTexture2D(char* data, UINT size)
	{
		return Ref<Texture2D>(new DirectX12Texture2D(data, size));
	}

	void DirectX12RendererAPI::SetTexture2D(Ref<Texture2D> tex, UINT slot, Stages stage)
	{
		auto& context = m_Core.GetDirectCommandContext();
		auto castTex2d = CastPtr<DirectX12Texture2D>(tex);
		D3D12_CPU_DESCRIPTOR_HANDLE descriptor = castTex2d->GetDescriptor();

		DirectX12ShaderResourceView view;
		view.m_View = descriptor;

		// TODO: Define way to set the index
		context.GetStateManager().SetSRV(stage, view, slot);
	}

	Ref<Sampler> DirectX12RendererAPI::CreateSampler(FilterType img, PaddingMethod padMode)
	{
		return Ref<Sampler>(new DirectX12Sampler(img, padMode));
	}

	void DirectX12RendererAPI::SetSampler(Ref<Sampler> sampler, Stages stage)
	{
		// TODO: Get Rid of Test code
		auto& context = m_Core.GetDirectCommandContext();
		auto castSampler = CastPtr<DirectX12Sampler>(sampler);
		D3D12_CPU_DESCRIPTOR_HANDLE descriptor = castSampler->GetDescriptor();

		DirectX12SamplerView view;
		view.m_View = descriptor;

		// TODO: Define way to set the index
		context.GetStateManager().SetSampler(stage, view, 0);
	}

	void DirectX12RendererAPI::SetTopology(TopologyType& type)
	{
		auto& context = m_Core.GetDirectCommandContext();
		auto& state = context.GetStateManager();
		D3D12_PRIMITIVE_TOPOLOGY d3Type = TranslateTopListType(TopologyType::Triangle);
		state.SetTop(d3Type);
	}

	void DirectX12RendererAPI::UpdatePipeline(PipelineStateInitializer& init)
	{
		// TODO: Grab current reference to root signature instead of recreating instance
		auto root = Ref<DirectX12RootSignature>(new DirectX12RootSignature());
		root->InitGenericRootSignature(D3D12_ROOT_SIGNATURE_FLAG_NONE);

		// TODO: Make rtv and depth format configurable
		D3D12_RT_FORMAT_ARRAY rtvFormats = {};
		rtvFormats.NumRenderTargets = 1;
		rtvFormats.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		CD3DX12_RASTERIZER_DESC rastDsc(D3D12_FILL_MODE_SOLID,
			D3D12_CULL_MODE_BACK,
			TRUE, D3D12_DEFAULT_DEPTH_BIAS,
			D3D12_DEFAULT_DEPTH_BIAS_CLAMP,
			D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
			TRUE, FALSE, FALSE,
			0,
			D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF);
		BufferLayoutBuilder layout = { {"POSITION", ShaderDataType::Float3}, {"UV_TEXCOORD", ShaderDataType::Float2} };
		DirectX12InputLayout dxLayout(layout);
		DirectX12PipelineArgs args = {
			{root->GetInternalRootSignature(),
			dxLayout.GetInternalLayout(),
			ProjectGE::TranslateTopType(init.toplopgyType),
			rastDsc,
			CD3DX12_SHADER_BYTECODE(CastPtr<DirectX12Shader>(init.shaderSet.shaders[Vertex])->ByteCode()),
			CD3DX12_SHADER_BYTECODE(CastPtr<DirectX12Shader>(init.shaderSet.shaders[Pixel])->ByteCode()),
			DXGI_FORMAT_D32_FLOAT,
			rtvFormats
			} 
		};

		auto stateRef = Ref<DirectX12PipelineState>(new DirectX12PipelineState());
		stateRef->Create(args);

		auto refData = Ref<DirectX12PipelineStateData>(new DirectX12PipelineStateData(stateRef, root));

		auto& state = m_Core.GetDirectCommandContext().GetStateManager();
		state.SetGraphicsPipelineState(refData);
	}

	void DirectX12RendererAPI::SubmitRecording()
	{
		m_Core.GetDirectCommandContext().FinalizeCommandList();
	}
};
