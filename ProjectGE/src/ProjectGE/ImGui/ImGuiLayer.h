#pragma once
#include "ProjectGE/Core/Layer.h"
#include "ProjectGE/Events/ApplicationEvents.h"
#include "ProjectGE/Events/KeyboardEvents.h"
#include "ProjectGE/Events/MouseEvents.h"

namespace ProjectGE {
	// A layer representing a ImGUI overlay, a GUI on top of the basic window
	class GE_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();
		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;

		void Begin() const;
		void End() const;
	};
};
