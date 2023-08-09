#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/BufferLayout.h"
#include "ProjectGE/Rendering/RenderAgnostics/PipelineDefiner.h"
#include "ProjectGE/Rendering/BindableResource.h"


namespace ProjectGE {
	// INTERFACE
	// A Buffer that holds Vertex information meant to be sent to a Render API pipeline
	class VertexBuffer : public BindableResource
	{
	public:
		virtual ~VertexBuffer() {}
		// Tells the VertexBuffer to generate vertex data definitions by converting the arguement into the RenderAPI specific formats
		virtual void AttachLayout(const BufferLayoutBuilder& layout) = 0;
		// Return the vertex data definition in the RenderAPI specific fromats
		virtual PipelineDefiner* GetLayout() = 0;
		// Instantiates an implementation of the VertexBuffer Interface (recommended to use this instead of instantiating a specific implementation)
		static VertexBuffer* Create(void* verticies, UINT count);
	};
};


