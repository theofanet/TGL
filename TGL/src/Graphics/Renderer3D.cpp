#include "tglpch.h"
#include "Renderer3D.h"
#include "Renderer.h"
#include "Core/Registry.h"
#include <glm/gtc/matrix_transform.hpp>


bool Renderer3D::s_Initialized = false;
Ref<VertexArray> Renderer3D::s_VA = nullptr;
Ref<Shader> Renderer3D::s_BasicShader = nullptr;
Ref<Shader> Renderer3D::s_TextureShader = nullptr;


void Renderer3D::Init() {
	const std::vector<float> vertices = {
		// front
		-1.0, -1.0,  1.0, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 1.0, -1.0,  1.0, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 1.0,  1.0,  1.0, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0,  1.0,  1.0, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		// top
		-1.0,  1.0,  1.0, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 1.0,  1.0,  1.0, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 1.0,  1.0, -1.0, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0,  1.0, -1.0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		// back
		 1.0, -1.0, -1.0, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-1.0, -1.0, -1.0, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-1.0,  1.0, -1.0, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 1.0,  1.0, -1.0, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 // bottom
		 -1.0, -1.0, -1.0, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		  1.0, -1.0, -1.0, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		  1.0, -1.0,  1.0, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		 -1.0, -1.0,  1.0, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		 // left
		 -1.0, -1.0, -1.0, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		 -1.0, -1.0,  1.0, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		 -1.0,  1.0,  1.0, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		 -1.0,  1.0, -1.0, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		 // right
		  1.0, -1.0,  1.0, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		  1.0, -1.0, -1.0, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		  1.0,  1.0, -1.0, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		  1.0,  1.0,  1.0, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f
	};

	const std::vector<uint32_t> indices = {
		// front
		0,  1,  2,
		2,  3,  0,
		// top
		4,  5,  6,
		6,  7,  4,
		// back
		8,  9, 10,
		10, 11,  8,
		// bottom
		12, 13, 14,
		14, 15, 12,
		// left
		16, 17, 18,
		18, 19, 16,
		// right
		20, 21, 22,
		22, 23, 20
	};

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

void Renderer3D::Shutdown() {

}

void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, float rotation) {
	glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
		* glm::rotate(glm::mat4(1.0f), rotation, { 1.0f, 1.0f, 1.0f })
		* glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });

	Ref<Material> material = CreateRef<Material>();
	material->SetAmbientMap(Renderer::GetWhiteTexture());
	material->SetColor(color);
	
	Renderer::Submit(s_TextureShader, s_VA, model, material);
}

void Renderer3D::DrawCube(const std::string& texturePath, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, float rotation) {
	glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
		* glm::rotate(glm::mat4(1.0f), rotation, { 1.0f, 0.0f, 1.0f })
		* glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });

	Ref<Material> material = CreateRef<Material>();
	material->SetAmbientMap(Registry::GetTexture(texturePath));
	material->SetColor(color);

	Renderer::Submit(s_TextureShader, s_VA, model, material);
}

void Renderer3D::DrawCube(const glm::vec3& position, const Ref<Material> material, const glm::vec3& size, float rotation){
	glm::mat4 model = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
		* glm::rotate(glm::mat4(1.0f), rotation, { 1.0f, 0.0f, 1.0f })
		* glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });

	Renderer::Submit(s_TextureShader, s_VA, model, material);
}


