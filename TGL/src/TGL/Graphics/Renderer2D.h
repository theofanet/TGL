#pragma once


#include "Buffers.h"
#include "Arrays.h"
#include "Shaders.h"
#include "Camera.h"


class Renderer2D {
public:
	static void Init();
	static void Shutdown();

	static void Begin(Ref<Camera> camera);
	static void End();
	static void Flush();
	static void FlushAndReset();

	static void DrawQuad(const glm::vec3& position, const glm::vec2& size = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f), float rotation = 0.0f, float textureIndex = 0.0f, float tilingFactor = 1.0f);
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f), float rotation = 0.0f, float textureIndex = 0.0f, float tilingFactor = 1.0f);
	static void DrawQuad(const std::string& texturePath, const glm::vec3& position, const glm::vec2& size = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f), float rotation = 0.0f, float tilingFactor = 1.0f);
	static void DrawQuad(const std::string& texturePath, const glm::vec2& position, const glm::vec2& size = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f), float rotation = 0.0f, float tilingFactor = 1.0f);

	static void ResetStats();

private:
	static bool s_Initialized;
	static Ref<VertexArray> s_VA;
	static Ref<VertexBuffer> s_VB;
	static Ref<Shader> s_BatchShader;

public:
	static uint32_t s_StatsDrawCalls, s_StatsQuads;
};