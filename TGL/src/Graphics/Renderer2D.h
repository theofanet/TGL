#pragma once


#include "Buffers.h"
#include "Arrays.h"
#include "Shaders.h"
#include "Material.h"


class Renderer2D {
public:
	static void Init();
	static void Shutdown();

	static void DrawQuad(const glm::vec2& position, const glm::vec2& size = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f), float rotation = 0.0f);
	static void DrawQuad(const std::string& texturePath, const glm::vec2& position, const glm::vec2& size = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f), float rotation=0.0f);

private:
	static bool s_Initialized;
	static Ref<VertexArray> s_VA;
	static Ref<Shader> s_BasicShader, s_TextureShader;
};