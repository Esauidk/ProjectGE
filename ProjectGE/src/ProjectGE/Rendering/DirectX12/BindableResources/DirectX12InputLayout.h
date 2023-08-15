#pragma once
#include "ProjectGE/Core/Core.h"

#include "ProjectGE/Rendering/RenderAgnostics/PipelineDefiner.h"
#include "ProjectGE/Rendering/RenderAgnostics/BufferLayout.h"

namespace ProjectGE {
	class DirectX12InputLayout : public PipelineDefiner
	{
	public:
		DirectX12InputLayout(const BufferLayoutBuilder& layout);
		void Append(PipelineStateObject& pipeline) override;
	private:
		Scope<D3D12_INPUT_ELEMENT_DESC[]> m_Desc;
		D3D12_INPUT_LAYOUT_DESC m_InputLayout;
	};
};


