#pragma once

#include <map>
#include "Core.h"
#include "Graphics/Shaders.h"


class Registry {
public:
	static Ref<Shader> CreateShader(const std::string& name, const std::string filePath);
	static Ref<Shader> GetShader(const std::string& name);

private:
	static std::map<std::string, Ref<Shader>> s_Shaders;
};