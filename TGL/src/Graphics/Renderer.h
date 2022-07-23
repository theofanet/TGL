#pragma once


#include <glm/glm.hpp>

#include "Core/Core.h"
#include "Buffers.h"
#include "Arrays.h"
#include "Shaders.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
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
		glm::vec3 ViewPosition;
		Renderer::Type Mode;
		glm::vec4 ClearColor;

		Context()
			: ViewProjectionMatrix(glm::mat4(1.0f)), Mode(R2D), ClearColor(.0f, .0f, .0f, 1.0f), ViewPosition(glm::vec3(0.0f)) {
		}
	};

	static void Init();
	static void Shutdown();

	inline static bool IsIntialized() { return s_IsInitialized; }

	static inline const Ref<Texture>GetWhiteTexture() { return s_WhiteTexture; }

	inline static void SetClearColor(const glm::vec3& color, float alpha = 1.0f) {
		s_Context->ClearColor = { color.r, color.g, color.b, alpha };
	}
	static void Clear();

	static void AddLight(Ref<Light> light);

	static void Begin(Type mode, Ref<Camera> camera);
	static void End();

	static void Submit(Ref<Shader> shader, Ref<VertexArray> vertexArray, const glm::mat4& model = glm::mat4(1.0f), const Ref<Material> material = nullptr);

private:
	static Ref<RendererEventHandler> s_Handler;
	static Ref<Context> s_Context;
	static Ref<Texture> s_WhiteTexture;
	static std::vector<Ref<Light>> s_Lights;
	static int s_LightsCount;
	static bool s_IsInitialized;

public:
	struct DrawStep {
		Ref<Shader> shader;
		Ref<VertexArray> va;
		glm::mat4 model;
		Ref<Material> material;

		DrawStep(Ref<Shader> sdr, Ref<VertexArray> vertexArray, const glm::mat4& mdl, const Ref<Material> mat)
			: shader(sdr), va(vertexArray), model(mdl), material(mat) {
		}
	};

	typedef std::map<Ref<Shader>, std::vector<DrawStep>> DrawStackType;

	class DrawStack {
	public:
		inline void PushStep(const DrawStep& s) {
			m_Stack[s.shader].push_back(s);
		}

		inline void Clear() { m_Stack.clear(); }

		inline void Draw(Ref<Context> context, const std::vector<Ref<Light>>& lights) const {
			for (auto& st : m_Stack) {
				st.first->Bind();
				st.first->SetMat4("u_ViewProjection", context->ViewProjectionMatrix);
				st.first->SetFloat("u_ViewPosition", context->ViewPosition);

				st.first->SetInt("u_LightsCount", lights.size());
				int i = 0;
				for (auto& light : lights) {
					st.first->SetFloat("u_Light[" + std::to_string(i) + "].ambient", light->GetAmbient());
					st.first->SetFloat("u_Light[" + std::to_string(i) + "].diffuse", light->GetDiffuse());
					st.first->SetFloat("u_Light[" + std::to_string(i) + "].specular", light->GetSpecular());
					st.first->SetFloat("u_Light[" + std::to_string(i) + "].direction", light->GetDirection());
					st.first->SetFloat("u_Light[" + std::to_string(i) + "].position", light->GetPosition());
					st.first->SetFloat("u_Light[" + std::to_string(i) + "].constant", light->GetConstant());
					st.first->SetFloat("u_Light[" + std::to_string(i) + "].linear", light->GetLinear());
					st.first->SetFloat("u_Light[" + std::to_string(i) + "].quadratic", light->GetQuadratic());
					st.first->SetFloat("u_Light[" + std::to_string(i) + "].cutOff", glm::cos(glm::radians(light->GetCutOff())));
					st.first->SetFloat("u_Light[" + std::to_string(i) + "].outerCutOff", glm::cos(glm::radians(light->GetOuterCutOff())));

					st.first->SetInt("u_Light[" + std::to_string(i) + "].type", light->GetType());

					i++;
				}

				for (auto& tv : st.second) {
					tv.material->GetAmbientMap()->Bind(0);
					st.first->SetInt("u_Material.diffuseMap", 0);

					tv.material->GetSpecularMap()->Bind(1);
					st.first->SetInt("u_Material.specularMap", 1);

					st.first->SetFloat("u_Material.color", tv.material->GetColor());
					st.first->SetFloat("u_Material.ambient", tv.material->GetAmbient());
					st.first->SetFloat("u_Material.diffuse", tv.material->GetDiffuse());
					st.first->SetFloat("u_Material.shininess", tv.material->GetShininess());
					st.first->SetFloat("u_Material.specular", tv.material->GetSpecular());

					st.first->SetMat4("u_Model", tv.model);
					tv.va->Draw();
				}
				st.first->Unbind();
			}
		}

	private:
		DrawStackType m_Stack;
	};

private:
	static DrawStack s_DrawStack;
};