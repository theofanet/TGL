#pragma once

#include <map>
#include "Core.h"
#include "Graphics/Shaders.h"
#include "Graphics/Texture.h"


class Registry {
public:
	static Ref<Shader> GetShader(const std::string& filePath);
	static Ref<Texture> GetTexture(const std::string& filePath);

	inline static void SetShaderPathPrefix(const std::string& prefix) { s_ShaderPathPrefix = prefix; }
	inline static void SetTexturePathPrefix(const std::string& prefix) { s_TexturePathPrefix = prefix; }

	inline static const std::string& GetShaderPathPrefix() { return s_ShaderPathPrefix; }
	inline static const std::string& GetTexturePathPrefix() { return s_TexturePathPrefix; }

private:
	static std::map<std::string, Ref<Shader>> s_Shaders;
	static std::map<std::string, Ref<Texture>> s_Textures;

	static std::string s_ShaderPathPrefix;
	static std::string s_TexturePathPrefix;
};