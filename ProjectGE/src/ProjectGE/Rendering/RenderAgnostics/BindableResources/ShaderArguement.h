#pragma once
#include "ProjectGE/Rendering/BindableResource.h"

namespace ProjectGE {
	enum class ShaderArguementType {
		None = 0,
		Constant,
		Reference,
		ReferenceTable
	};
	class ShaderArguement : public BindableResource
	{
		static ShaderArguement* Create(const void* data, const ShaderArguementType type);
	};
};


