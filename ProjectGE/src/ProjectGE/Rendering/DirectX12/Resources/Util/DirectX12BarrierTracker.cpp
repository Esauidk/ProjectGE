#include "gepch.h"
#include "DirectX12BarrierTracker.h"

#include <d3d12.h>
#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"

#include "ProjectGE/Core/Log.h"

namespace ProjectGE {

	void DirectX12BarrierTracker::Init(D3D12_RESOURCE_STATES initialState, ID3D12Resource* resource)
	{
		m_CurrentState = initialState;
		m_LastState = initialState;
		m_Resource = resource;
		m_IsInit = true;
	}

	void DirectX12BarrierTracker::TransitionBarrier(D3D12_RESOURCE_STATES nextState)
	{
		GE_CORE_ASSERT(m_IsInit, "Transition Barrier Has Not Been Initialized Yet");

		if (m_CurrentState != nextState) {
			auto& core = DirectX12Core::GetCore();
			auto& cmdList = core.GetDirectCommandContext().GetCommandList();
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
				m_Resource,
				m_CurrentState, nextState
			);
			cmdList->ResourceBarrier(1, &barrier);
			m_LastState = m_CurrentState;
			m_CurrentState = nextState;
		}
	}

	void DirectX12BarrierTracker::UndoTransition(ID3D12Resource* resource)
	{
		TransitionBarrier(m_LastState);
	}
};

