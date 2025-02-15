#pragma once
#include "GameSmithEngine.h"
#include "EditorWindow.h"

namespace GameSmithEditor {
	class GameViewLayer : public EditorWindow {
	public:
		GameViewLayer();
		static inline void SetScreenTexture(GameSmith::Ref<GameSmith::RenderTexture> screenTex) { m_GameScreenTexture = screenTex; };
		void OnImGuiRender() override;
		void OnUpdate(float dt) override;
	private:
		static GameSmith::Ref<GameSmith::RenderTexture> m_GameScreenTexture;
		GameSmith::Ref<GameSmith::RenderTexture> m_GameViewTexture;
		const GameSmith::ImGuiTextureSpace* m_GameViewHandle;
	};
};

