#pragma once


#include <glm/glm.hpp>

#include "Core/Core.h"
#include "Buffers.h"
#include "Arrays.h"
#include "Shaders.h"
#include "Texture.h"
#include "Camera.h"
#include "Core/Events.h"

class RendererEventHandler {
public:
	RendererEventHandler();
	bool OnWindowResize(EventWindowResize& e);
};


class Renderer {
public:
	enum Type { R2D, R3D };

	struct Context {
		glm::mat4 ViewProjectionMatrix;
		Renderer::Type Mode;
		glm::vec4 ClearColor;

		Context()
			: ViewProjectionMatrix(glm::mat4(1.0f)), Mode(R2D), ClearColor(0.3f, 0.3, 0.3, 1.0f) {
		}
	};

	static void Init();
	static void Shutdown();

	inline static void SetClearColor(const glm::vec3& color, float alpha = 1.0f) {
		s_Context->ClearColor = { color.r, color.g, color.b, alpha };
	}
	static void Clear();


	static void Begin(Type mode, Ref<Camera> camera);
	static void End();

	static void Submit(Ref<Shader> shader, Ref<VertexArray> vertexArray, const glm::mat4& model = glm::mat4(1.0f), Ref<Texture> texture = nullptr, int slot = 0, const glm::vec4& color = glm::vec4(1.0f));

private:
	static Ref<RendererEventHandler> s_Handler;
	static Ref<Context> s_Context;

public:
	struct DrawStep {
		Ref<Shader> shader;
		Ref<Texture> texture;
		Ref<VertexArray> va;
		glm::mat4 model;
		int textureSlot;
		glm::vec4 color;

		DrawStep(Ref<Shader> sdr, Ref<VertexArray> vertexArray, const glm::mat4& mdl, Ref<Texture> text, int slot, const glm::vec4 & clr)
			: texture(text), shader(sdr), va(vertexArray), model(mdl), textureSlot(slot), color(clr) {
		}
	};

	typedef std::map<Ref<Shader>, std::map<std::pair<Ref<Texture>, int>, std::vector<DrawStep>>> DrawStackType;

	class DrawStack {
	public:
		inline void PushStep(const DrawStep& s) {
			m_Stack[s.shader][std::make_pair(s.texture, s.textureSlot)].push_back(s);
		}

		inline void Clear() { m_Stack.clear(); }

		inline void Draw(Ref<Context> context) const {
			for (auto& st : m_Stack) {
				st.first->Bind();
				st.first->SetMat4("u_ViewProjection", context->ViewProjectionMatrix);

				for (auto& tv : st.second) {
					if (tv.first.first) 
						tv.first.first->Bind(tv.first.second);

					for (auto& step : tv.second) {
						st.first->SetMat4("u_Model", step.model);
						st.first->SetFloat("u_Color", step.color.r, step.color.g, step.color.b, step.color.a);

						step.va->Draw();
					}
				}
			}
		}

	private:
		DrawStackType m_Stack;
	};

private:
	static DrawStack s_DrawStack;
};