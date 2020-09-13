#include "tglpch.h"
#include "Registry.h"


std::map<std::string, Ref<Shader>> Registry::s_Shaders;
std::map<std::string, Ref<Texture>> Registry::s_Textures;
std::map<std::string, Ref<Material>> Registry::s_Materials;

std::string Registry::s_ShaderPathPrefix = "";
std::string Registry::s_TexturePathPrefix = "";


Ref<Shader> Registry::GetShader(const std::string& filePath){
	std::string f = s_ShaderPathPrefix + filePath;
	if (s_Shaders.find(f) == s_Shaders.end())
		return s_Shaders[f] = Shader::Create(f);
	return s_Shaders[f];
}

Ref<Texture> Registry::GetTexture(const std::string& filePath){
	std::string f = s_TexturePathPrefix + filePath;
	if (s_Textures.find(f) == s_Textures.end())
		s_Textures[f] = Texture::Create(f);
	return s_Textures[f];
}

const Ref<Material> Registry::GetMaterial(const std::string& key){
	if (s_Materials.find(key) == s_Materials.end())
		s_Materials[key] = CreateRef<Material>();
	return s_Materials[key];
}

void Registry::SetMaterial(const std::string& key, const Ref<Material> material){
	s_Materials[key] = material;
}

void Registry::SetTexture(const std::string& key, const Ref<Texture> texture){
	std::string f = s_TexturePathPrefix + key;
	s_Textures[f] = texture;
}
