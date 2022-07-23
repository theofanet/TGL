#include "tglpch.h"
#include "Renderer2D.h"
#include "Renderer.h"
#include "Core/Registry.h"
#include <glm/gtc/matrix_transform.hpp>


bool Renderer2D::s_Initialized = false;
Ref<VertexArray> Renderer2D::s_VA = nullptr;
Ref<VertexBuffer> Renderer2D::s_VB = nullptr;
Ref<Shader> Renderer2D::s_BasicShader = nullptr;
Ref<Shader> Renderer2D::s_TextureShader = nullptr;
Ref<Shader> Renderer2D::s_BatchShader = nullptr;
uint32_t Renderer2D::s_StatsDrawCalls = 0;
uint32_t Renderer2D::s_StatsQuads = 0;

struct Vertex {
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoords;
	float TexIndex;
	float TilingFactor;
};

struct Renderer2DData {
	static const uint32_t MaxQuads = 10000;
	static const uint32_t MaxVertices = MaxQuads * 4;
	static const uint32_t MaxIndices = MaxQuads * 6;
	static const uint32_t MaxTextureSlots = 32;
	
	uint32_t IndexCount = 0;
	Vertex* VertexBufferBase = nullptr;
	Vertex* VertexBufferPtr = nullptr;

	glm::vec4 QuadVertexPositions[4];
	glm::vec2 TextureCoords[4];

	std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
	uint32_t TextureSlotIndex = 1;
};

static Renderer2DData s_Data;

void Renderer2D::Init() {
	s_VB = VertexBuffer::Create(Renderer2DData::MaxVertices * sizeof(Vertex));
	s_VB->AddAttrib(3, GL_FLOAT); // x y z
	s_VB->AddAttrib(4, GL_FLOAT); // r g b a
	s_VB->AddAttrib(2, GL_FLOAT); // u v
	s_VB->AddAttrib(1, GL_FLOAT); // texture ID
	s_VB->AddAttrib(1, GL_FLOAT); // tiling Factor

	s_Data.VertexBufferBase = new Vertex[Renderer2DData::MaxVertices];

	uint32_t* indices = new uint32_t[Renderer2DData::MaxIndices];

	uint32_t offset = 0;
	for (uint32_t i = 0; i < Renderer2DData::MaxIndices; i += 6) {
		indices[i + 0] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;
		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;
		offset += 4;
	}

	Ref<IndexBuffer> ib = IndexBuffer::Create(indices, Renderer2DData::MaxIndices);

	s_VA = VertexArray::Create();
	s_VA->AddVertexBuffer(s_VB);
	s_VA->SetIndexBuffer(ib);

	delete[] indices;

	std::string oldPrefix = Registry::GetShaderPathPrefix();
	Registry::SetShaderPathPrefix("");
	s_BasicShader = Registry::GetShader("assets/shaders/shader.glsl");
	s_TextureShader = Registry::GetShader("assets/shaders/texturenolight.glsl");
	s_BatchShader = Registry::GetShader("assets/shaders/batch.glsl");
	Registry::SetShaderPathPrefix(oldPrefix);

	s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

	s_Data.TextureCoords[0] = { 0.0f, 0.0f };
	s_Data.TextureCoords[1] = { 1.0f, 0.0f };
	s_Data.TextureCoords[2] = { 1.0f, 1.0f };
	s_Data.TextureCoords[3] = { 0.0f, 1.0f };

	int32_t samplers[Renderer2DData::MaxTextureSlots];
	for (uint32_t i = 0; i < Renderer2DData::MaxTextureSlots; i++)
		samplers[i] = i;

	s_BatchShader->Bind();
	s_BatchShader->SetIntArray("u_Textures", samplers, Renderer2DData::MaxTextureSlots);

	s_Data.TextureSlots[0] = Renderer::GetWhiteTexture();

	s_Initialized = true;
}

void Renderer2D::Shutdown(){
	//delete[] s_Data.VertexBufferBase; TOdO : FIX ?
}

void Renderer2D::Begin(Ref<Camera> camera) {
	s_BatchShader->Bind();
	s_BatchShader->SetMat4("u_ViewProjection", camera->GetViewProjectionMatrix());

	s_Data.IndexCount = 0;
	s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
	s_Data.TextureSlotIndex = 1;
}

void Renderer2D::End() {
	uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.VertexBufferPtr - (uint8_t*)s_Data.VertexBufferBase);
	s_VB->SetData(s_Data.VertexBufferBase, dataSize);

	Flush();
}

void Renderer2D::Flush() {
	if (s_Data.IndexCount == 0)
		return;

	for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		s_Data.TextureSlots[i]->Bind(i);

	s_VA->Draw(s_Data.IndexCount);
	s_StatsDrawCalls++;
}

void Renderer2D::FlushAndReset() {
	End();

	s_Data.IndexCount = 0;
	s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
	s_Data.TextureSlotIndex = 1;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation, float textureIndex, float tilingFactor) {
	DrawQuad({ position.x, position.y, 0.0f }, size, color, rotation, textureIndex, tilingFactor);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation, float textureIndex, float tilingFactor) {
	if (s_Data.IndexCount >= Renderer2DData::MaxIndices)
		FlushAndReset();

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
		* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	for (size_t i = 0; i < 4; i++) {
		s_Data.VertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
		s_Data.VertexBufferPtr->Color = color;
		s_Data.VertexBufferPtr->TexCoords = s_Data.TextureCoords[i];
		s_Data.VertexBufferPtr->TexIndex = textureIndex;
		s_Data.VertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.VertexBufferPtr++;
	}

	s_Data.IndexCount += 6;
	s_StatsQuads++;
}


void Renderer2D::DrawQuad(const std::string& texturePath, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation, float tilingFactor) {
	DrawQuad(texturePath, { position.x, position.y, 0.0f }, size, color, rotation, tilingFactor);
}

void Renderer2D::DrawQuad(const std::string& texturePath, const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation, float tilingFactor){
	Ref<Texture> texture = Registry::GetTexture(texturePath);
	float textureIndex = 0.0f;
	
	for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
		if (*s_Data.TextureSlots[i].get() == *texture.get()) {
			textureIndex = (float)i;
			break;
		}
	}

	if (textureIndex == 0.0f) {
		textureIndex = (float)s_Data.TextureSlotIndex;
		s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
		s_Data.TextureSlotIndex++;
	}

	DrawQuad(position, size, color, rotation, textureIndex, tilingFactor);
}

void Renderer2D::ResetStats() {
	s_StatsDrawCalls = 0;
	s_StatsQuads = 0;
}