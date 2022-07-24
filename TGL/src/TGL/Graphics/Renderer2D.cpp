#include "tglpch.h"
#include "Renderer2D.h"
#include "Renderer.h"
#include "TGL/Core/Registry.h"
#include <glm/gtc/matrix_transform.hpp>


bool Renderer2D::s_Initialized = false;
Ref<VertexArray> Renderer2D::s_QuadsVA = nullptr;
Ref<VertexBuffer> Renderer2D::s_QuadsVB = nullptr;
Ref<VertexArray> Renderer2D::s_LinesVA = nullptr;
Ref<VertexBuffer> Renderer2D::s_LinesVB = nullptr;
Ref<Shader> Renderer2D::s_BatchShader = nullptr;
Ref<Shader> Renderer2D::s_LineShader = nullptr;
uint32_t Renderer2D::s_StatsDrawCalls = 0;
uint32_t Renderer2D::s_StatsQuads = 0;

struct Vertex {
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoords;
	float TexIndex;
	float TilingFactor;
};

struct LineVertex {
	glm::vec3 Position;
	glm::vec4 Color;
};

struct Renderer2DData {
	static const uint32_t MaxQuads = 10000;
	static const uint32_t MaxVertices = MaxQuads * 4;
	static const uint32_t MaxIndices = MaxQuads * 6;
	static const uint32_t MaxTextureSlots = 32;
	
	uint32_t QuadIndexCount = 0;
	Vertex* VertexBufferBase = nullptr;
	Vertex* VertexBufferPtr = nullptr;

	uint32_t LineVertexCount = 0;
	LineVertex* LineVertexBufferBase = nullptr;
	LineVertex* LineVertexBufferPtr = nullptr;

	glm::vec4 QuadVertexPositions[4];
	glm::vec2 TextureCoords[4];

	std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
	uint32_t TextureSlotIndex = 1;
	
	glm::mat4 CameraViewProjectionMatrix;
};

static Renderer2DData s_Data;

void Renderer2D::Init() {
	s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

	s_Data.TextureCoords[0] = { 0.0f, 0.0f };
	s_Data.TextureCoords[1] = { 1.0f, 0.0f };
	s_Data.TextureCoords[2] = { 1.0f, 1.0f };
	s_Data.TextureCoords[3] = { 0.0f, 1.0f };

	/* Quands */
	s_QuadsVB = VertexBuffer::Create(Renderer2DData::MaxVertices * sizeof(Vertex));
	s_QuadsVB->AddAttrib(3, GL_FLOAT); // x y z
	s_QuadsVB->AddAttrib(4, GL_FLOAT); // r g b a
	s_QuadsVB->AddAttrib(2, GL_FLOAT); // u v
	s_QuadsVB->AddAttrib(1, GL_FLOAT); // texture ID
	s_QuadsVB->AddAttrib(1, GL_FLOAT); // tiling Factor

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

	s_QuadsVA = VertexArray::Create();
	s_QuadsVA->AddVertexBuffer(s_QuadsVB);
	s_QuadsVA->SetIndexBuffer(ib);

	delete[] indices;

	/* Lines */
	s_Data.LineVertexBufferBase = new LineVertex[Renderer2DData::MaxVertices];

	s_LinesVB = VertexBuffer::Create(Renderer2DData::MaxVertices * sizeof(LineVertex));
	s_LinesVB->AddAttrib(3, GL_FLOAT); // x y z
	s_LinesVB->AddAttrib(4, GL_FLOAT); // r g b a

	s_LinesVA = VertexArray::Create();
	s_LinesVA->AddVertexBuffer(s_LinesVB);

	/* Shaders And Textures */
	std::string oldPrefix = Registry::GetShaderPathPrefix();
	Registry::SetShaderPathPrefix("");
	s_BatchShader = Registry::GetShader("assets/shaders/batch.glsl");
	s_LineShader = Registry::GetShader("assets/shaders/batch_lines.glsl");
	Registry::SetShaderPathPrefix(oldPrefix);

	int32_t samplers[Renderer2DData::MaxTextureSlots];
	for (uint32_t i = 0; i < Renderer2DData::MaxTextureSlots; i++)
		samplers[i] = i;

	s_BatchShader->Bind();
	s_BatchShader->SetIntArray("u_Textures", samplers, Renderer2DData::MaxTextureSlots);

	s_Data.TextureSlots[0] = Renderer::GetWhiteTexture();

	s_Initialized = true;
}

void Renderer2D::Shutdown(){
	// delete[] s_Data.VertexBufferBase; // TOdO : FIX ?
	// delete[] s_Data.LineVertexBufferBase; // TOdO : FIX ?
}

void Renderer2D::Begin(RawCamera camera, glm::mat4 transform){
	s_Data.CameraViewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);
	StartBatch();
}

void Renderer2D::Begin(Ref<Camera> camera) {
	s_Data.CameraViewProjectionMatrix = camera->GetViewProjectionMatrix();
	StartBatch();
}

void Renderer2D::End() {
	Flush();
}

void Renderer2D::Flush() {
	if (s_Data.QuadIndexCount > 0) {
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.VertexBufferPtr - (uint8_t*)s_Data.VertexBufferBase);
		s_QuadsVB->SetData(s_Data.VertexBufferBase, dataSize);
		s_BatchShader->Bind();
		s_BatchShader->SetMat4("u_ViewProjection", s_Data.CameraViewProjectionMatrix);
		s_QuadsVA->Draw(s_Data.QuadIndexCount);
		s_StatsDrawCalls++;
	}

	if (s_Data.LineVertexCount > 0) {
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
		s_LinesVB->SetData(s_Data.LineVertexBufferBase, dataSize);
		s_LineShader->Bind();
		s_LineShader->SetMat4("u_ViewProjection", s_Data.CameraViewProjectionMatrix);
		Renderer::SetLineWidth(2.0f);
		Renderer::DrawVertexArray(s_LinesVA, s_Data.LineVertexCount, GL_LINES);
		s_StatsDrawCalls++;
	}
}

void Renderer2D::FlushAndReset() {
	End();
	StartBatch();
}

void Renderer2D::StartBatch(){
	s_Data.QuadIndexCount = 0;
	s_Data.VertexBufferPtr = s_Data.VertexBufferBase;

	s_Data.LineVertexCount = 0;
	s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

	s_Data.TextureSlotIndex = 1;
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, float textureIndex, float tilingFactor) {
	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		FlushAndReset();

	for (size_t i = 0; i < 4; i++) {
		s_Data.VertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
		s_Data.VertexBufferPtr->Color = color;
		s_Data.VertexBufferPtr->TexCoords = s_Data.TextureCoords[i];
		s_Data.VertexBufferPtr->TexIndex = textureIndex;
		s_Data.VertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.VertexBufferPtr++;
	}

	s_Data.QuadIndexCount += 6;
	s_StatsQuads++;
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const std::string& texturePath, const glm::vec4& color, float tilingFactor) {
	DrawQuad(transform, color, GetTextureIndex(texturePath), tilingFactor);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation, float textureIndex, float tilingFactor) {
	DrawQuad({ position.x, position.y, 0.0f }, size, color, rotation, textureIndex, tilingFactor);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation, float textureIndex, float tilingFactor) {
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
		* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	DrawQuad(transform, color, textureIndex, tilingFactor);
}


void Renderer2D::DrawQuad(const std::string& texturePath, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation, float tilingFactor) {
	DrawQuad(texturePath, { position.x, position.y, 0.0f }, size, color, rotation, tilingFactor);
}

void Renderer2D::DrawQuad(const std::string& texturePath, const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation, float tilingFactor){
	DrawQuad(position, size, color, rotation, GetTextureIndex(texturePath), tilingFactor);
}

void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color){
	s_Data.LineVertexBufferPtr->Position = p0;
	s_Data.LineVertexBufferPtr->Color = color;
	s_Data.LineVertexBufferPtr++;

	s_Data.LineVertexBufferPtr->Position = p1;
	s_Data.LineVertexBufferPtr->Color = color;
	s_Data.LineVertexBufferPtr++;

	s_Data.LineVertexCount += 2;
}

void Renderer2D::ResetStats() {
	s_StatsDrawCalls = 0;
	s_StatsQuads = 0;
}

float Renderer2D::GetTextureIndex(const std::string& filepath) {
	Ref<Texture> texture = Registry::GetTexture(filepath);
	float textureIndex = 0.0f;

	for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
		if (*s_Data.TextureSlots[i].get() == *texture.get()) {
			textureIndex = (float)i;
			break;
		}
	}

	if (textureIndex == 0.0f) {
		if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
			FlushAndReset();

		textureIndex = (float)s_Data.TextureSlotIndex;
		s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
		s_Data.TextureSlotIndex++;
	}

	return textureIndex;
}