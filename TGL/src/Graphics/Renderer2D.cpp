#include "tglpch.h"
#include "Renderer2D.h"
#include "Renderer.h"
#include "Core/Registry.h"
#include <glm/gtc/matrix_transform.hpp>


bool Renderer2D::s_Initialized = false;
Ref<VertexArray> Renderer2D::s_VA = nullptr;
Ref<Shader> Renderer2D::s_BasicShader = nullptr;
Ref<Shader> Renderer2D::s_TextureShader = nullptr;


void Renderer2D::Init(){
	const std::vector<float> vertices = {
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f
	};

	const std::vector<uint32_t> indices = { 0, 1, 3, 3, 1, 2 };

	Ref<VertexBuffer> vb = VertexBuffer::Create(vertices);
	vb->AddAttrib(3, GL_FLOAT); // x y z
	vb->AddAttrib(2, GL_FLOAT); // u v
	vb->AddAttrib(3, GL_FLOAT); // normals x y z

	Ref<IndexBuffer> ib = IndexBuffer::Create(indices);

	s_VA = VertexArray::Create();
	s_VA->AddVertexBuffer(vb);
	s_VA->SetIndexBuffer(ib);

	std::string oldPrefix = Registry::GetShaderPathPrefix();
	Registry::SetShaderPathPrefix("");
	s_BasicShader = Registry::GetShader("assets/shaders/shader.glsl");
	s_TextureShader = Registry::GetShader("assets/shaders/texture.glsl");
	Registry::SetShaderPathPrefix(oldPrefix);

	s_Initialized = true;
}

void Renderer2D::Shutdown(){

}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation){
	glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, 1.0 })
		* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
		* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	Ref<Material> material = CreateRef<Material>();
	material->SetAmbientMap(Renderer::GetWhiteTexture());
	material->SetColor(color);

	Renderer::Submit(s_TextureShader, s_VA, model, material);
}

void Renderer2D::DrawQuad(const std::string& texturePath, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation){
	glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, 1.0 })
		* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
		* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	Ref<Material> material = CreateRef<Material>();
	material->SetAmbientMap(Registry::GetTexture(texturePath));
	material->SetColor(color);

	Renderer::Submit(s_TextureShader, s_VA, model, material);
}


