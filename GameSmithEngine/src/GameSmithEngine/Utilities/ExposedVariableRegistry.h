#pragma once
#include "ParameterContainer.h"

namespace GameSmith {
	enum ExposedVariableFlags {
		Asset = 0x1
	};

	struct ExposedVariableEntry {
		void* originalVariableRef;
		ContainerDataType variableDataType;
		unsigned int flags;
	};

	class ExposedVariableRegistry
	{
	public:
		inline void AddExposedVariable(std::string variableName, ExposedVariableEntry entry) { if (entry.originalVariableRef == nullptr) return; m_Registry.insert({ variableName, entry }); }
		void GenerateVariableMap(std::unordered_map<std::string, Ref<ParameterContainer>>* outMap);
		void BootstrapFromValueMap(const std::unordered_map<std::string, Ref<ParameterContainer>>& inMap);
	private:
		std::unordered_map<std::string, ExposedVariableEntry> m_Registry;
	};
};


