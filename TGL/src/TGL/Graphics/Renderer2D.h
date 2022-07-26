#pragma once


#include "Buffers.h"
#include "Arrays.h"
#include "Shaders.h"
#include "Texture.h"
#include "EditorCamera.h"


class Renderer2D {
public:
	static void Init();
	static void Shutdown();

	static void Begin(const EditorCamera& camera);
	static void Begin(const RawCamera& camera, const glm::mat4& transform);
	static void Begin(const Ref<Camera>& camera);
	static void End();
	static void Flush();
	static void FlushAndReset();

	static void StartBatch();

	static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, float textureIndex = 0.0f, float tilingFactor = 1.0f);
	static void DrawQuad(const glm::mat4& transform, const std::string& texturePath, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
	static void DrawQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

	static void DrawQuad(const glm::vec3& position, const glm::vec2& size = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f), float rotation = 0.0f, float textureIndex = 0.0f, float tilingFactor = 1.0f);
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f), float rotation = 0.0f, float textureIndex = 0.0f, float tilingFactor = 1.0f);
	static void DrawQuad(const std::string& texturePath, const glm::vec3& position, const glm::vec2& size = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f), float rotation = 0.0f, float tilingFactor = 1.0f);
	static void DrawQuad(const std::string& texturePath, const glm::vec2& position, const glm::vec2& size = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f), float rotation = 0.0f, float tilingFactor = 1.0f);

	static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);

	static void ResetStats();

	static float GetTextureIndex(const std::string& filepath);
	static float GetTextureIndex(const Ref<Texture>& texture);

private:
	static bool s_Initialized;
	static Ref<VertexArray> s_QuadsVA, s_LinesVA;
	static Ref<VertexBuffer> s_QuadsVB, s_LinesVB;
	static Ref<Shader> s_BatchShader, s_LineShader;

public:
	static uint32_t s_StatsDrawCalls, s_StatsQuads;
};