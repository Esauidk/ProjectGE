#pragma once

#include "ProjectGE/Rendering/DirectX12/DirectX12PipelineState.h"
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12RootSignature.h"

namespace ProjectGE {
	class DirectX12StateManager
	{
	public:
		Ref<DirectX12PipelineStateData> GetGraphicsPiplineState() { return PipelineState.Graphics.CurPipelineData; }
		void SetGraphicsPipelineState(Ref<DirectX12PipelineStateData> pipelineData);
		void BindState();
	private:
		void LowLevelSetGraphicsPipelineState(Ref<DirectX12PipelineState> pipeline);
		void LowLevelSetRootSignature(Ref<DirectX12RootSignature> root);

		struct {
			struct {
				Ref<DirectX12PipelineStateData> CurPipelineData = nullptr;
				bool updateRootSignature = false;
			} Graphics = {};

			struct {
				Ref<DirectX12PipelineState> CurPipeline = nullptr;
			} Basic = {};
			
		} PipelineState = {};


	};
};


