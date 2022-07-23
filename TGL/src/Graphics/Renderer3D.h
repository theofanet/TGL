#pragma once


#include "Buffers.h"
#include "Arrays.h"
#include "Shaders.h"
#include "Material.h"
#include "Objects/Model.h"


class Renderer3D {
public:
	static void Init();
	static void Shutdown();

	static void DrawCube(const glm::vec3& position, const glm::vec3& size = glm::vec4(1.0f), const glm::vec4& color = glm::vec4(1.0f), float rotation = 0.0f);
	static void DrawCube(const std::string& texturePath, const glm::vec3& position, const glm::vec3& size = glm::vec4(1.0f), const glm::vec4& color = glm::vec4(1.0f), float rotation = 0.0f);
	static void DrawCube(const glm::vec3& position, const Ref<Material> material, const glm::vec3& size = glm::vec4(1.0f), float rotation = 0.0f);
	static void DrawCube(const glm::vec3& position, const Ref<Texture> texture, const glm::vec3& size = glm::vec4(1.0f), float rotation = 0.0f);

	static void DrawModel(Ref<Model> model, const glm::vec3& position, const glm::vec3& size = glm::vec4(1.0f), float rotation = 0.0f);

	static void DrawVertexArray(const Ref<VertexArray> va, const glm::vec3& position, const glm::vec3& size = glm::vec4(1.0f), const glm::vec4& color = glm::vec4(1.0f), float rotation = 0.0f);

private:
	static bool s_Initialized;
	static Ref<VertexArray> s_VA;
	static Ref<Shader> s_BasicShader, s_TextureShader;
};