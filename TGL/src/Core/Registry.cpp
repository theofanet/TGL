#include "tglpch.h"
#include "Registry.h"


std::map<std::string, Ref<Shader>> Registry::s_Shaders;


Ref<Shader> Registry::CreateShader(const std::string& name, const std::string filePath){
	Ref<Shader> shader = Shader::Create(filePath);
	s_Shaders[name] = shader;
	return shader;
}

Ref<Shader> Registry::GetShader(const std::string& name){
	if (s_Shaders.find(name) != s_Shaders.end())
		return s_Shaders[name];
	return nullptr;
}
