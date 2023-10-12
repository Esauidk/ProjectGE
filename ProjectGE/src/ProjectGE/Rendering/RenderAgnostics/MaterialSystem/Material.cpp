#include "gepch.h"
#include "Material.h"

#include "ProjectGE/Rendering/RenderingManager.h"

namespace ProjectGE {
	Material::Material(std::vector<std::string> order,
		std::unordered_map<std::string, Ref<ShaderParameter>> paramters,
		std::unordered_map<std::string, Ref<TextureAsset>> textures) :
		m_ParameterKeys(order),
		m_Paramters(paramters),
		m_Textures(textures),
		m_ParameterByteTotal(0)
	{
		for (auto& entry : m_Paramters) {
			m_ParameterByteTotal += entry.second->GetSize();
		}

		auto renderManager = RenderingManager::GetInstance();

		GE_CORE_ASSERT(renderManager != nullptr, "Render manager is not running! Required for material to be used");

		m_GPULocation = renderManager->GetRenderAPI()->CreateConstantBuffer(m_ParameterByteTotal);
	}

	void Material::ApplyMaterial() {
		// TODO: Attach shaders

		char* temp = new char[m_ParameterByteTotal];
		char* cur = temp;
		for (auto& entry : m_ParameterKeys) {
			auto pair = m_Paramters.find(entry);
			char* ptr = pair->second->GetCharData();
			unsigned int bytes = pair->second->GetSize();
			memcpy(cur, ptr, bytes);
			cur += bytes;
		}

		m_GPULocation->UpdateData((BYTE*)temp, m_ParameterByteTotal);

		delete[] temp;

		auto renderManager = RenderingManager::GetInstance();

		GE_CORE_ASSERT(renderManager != nullptr, "Render manager is not running! Required for material to be used");

		renderManager->GetRenderAPI()->SetConstantBuffer(m_GPULocation, STAGE_VERTEX, ShaderConstantType::Instance);
		renderManager->GetRenderAPI()->SetConstantBuffer(m_GPULocation, STAGE_PIXEL, ShaderConstantType::Instance);

		// TODO: Attach textures
	}
};
