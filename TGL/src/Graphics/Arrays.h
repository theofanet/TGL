#pragma once


#include <glad/glad.h>
#include <vector>
#include <Core/Core.h>
#include <Graphics/Buffers.h>


class VertexArray {
public:
	static Ref<VertexArray> Create();

	VertexArray();
	virtual ~VertexArray();

	void Bind();
	void Unbind();

	void Draw();

	void AddVertexBuffer(const Ref<VertexBuffer>& buffer);
	void SetIndexBuffer(const Ref<IndexBuffer>& buffer);

	inline const Ref<IndexBuffer>& GetIndexBuffer() { return m_IB; }

private:
	GLuint m_ID;

	std::vector<Ref<VertexBuffer>> m_VBs;
	Ref<IndexBuffer> m_IB;
};