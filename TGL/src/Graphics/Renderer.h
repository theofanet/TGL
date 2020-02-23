#pragma once


#include <glm/glm.hpp>

#include "Core/Core.h"
#include "Buffers.h"
#include "Arrays.h"
#include "Shaders.h"


class Renderer {
public:
	enum Type { R2D, R3D };

	static void Init();
	static void Shutdown();

	inline static void SetClearColor(const glm::vec3& color, float alpha = 1.0f) {
		s_Context->ClearColor = { color.r, color.g, color.b, alpha };
	}
	static void Clear();

	static void Begin(Type mode);
	static void End();

	static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& model = glm::mat4(1.0f));

private:
	struct Context {
		glm::mat4 ViewProjectionMatrix;
		Renderer::Type Mode;
		glm::vec4 ClearColor;

		Context() 
			: ViewProjectionMatrix(glm::mat4(1.0f)), Mode(R2D), ClearColor(0.3f, 0.3, 0.3, 1.0f) {
		}
	};

	static Ref<Context> s_Context;
};