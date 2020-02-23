#pragma once


#include "Buffers.h"
#include "Arrays.h"
#include "Shaders.h"


class Renderer2D {
public:
	static void Init();
	static void Shutdown();

	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation=0.0f);

private:
	static bool s_Initialized;
	static Ref<VertexArray> s_VA;
	static Ref<Shader> s_BasicShader;
};