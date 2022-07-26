#pragma once

#include <glad/glad.h>
#include <vector>

#include "TGL/Core/Core.h"



uint32_t GetTypeSize(GLenum type);

class Buffer {
public:
	Buffer(GLenum type, const void* data, uint32_t size, uint32_t count, GLenum usage = GL_STATIC_DRAW);
	virtual ~Buffer();

	virtual const void Bind() { glBindBuffer(m_BufferType, m_ID); };
	virtual const void Unbind() { glBindBuffer(m_BufferType, 0); };

	virtual void SetData(const void* data, uint32_t size);

	inline uint32_t GetCount() { return m_Count; }
	inline uint32_t GetSize() { return m_Size; }

protected:
	GLuint m_ID;
	GLenum m_BufferType, m_BufferUsage;

	uint32_t m_Size, m_Count;
};


class VertexBuffer : public Buffer {
public:
	struct Attrib {
		uint32_t index;
		uint32_t size;
		GLenum type;
		bool normalized;
		uint32_t offset;
	};

	static Ref<VertexBuffer> Create(const std::vector<float>& vertices);
	static Ref<VertexBuffer> Create(const uint32_t size);

	VertexBuffer(const std::vector<float>& vertices);
	VertexBuffer(const uint32_t size);

	void AddAttrib(uint32_t count, GLenum type, bool normalized = false);
	inline const std::vector<Attrib>& GetAttribs() { return m_Attribs; }

	inline uint32_t GetStride() { return m_Stride; }

protected:
	uint32_t m_Stride;
	std::vector<Attrib> m_Attribs;
};


class IndexBuffer : public Buffer {
public:
	static Ref<IndexBuffer> Create(const std::vector<uint32_t>& indices);
	static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);

	IndexBuffer(const std::vector<uint32_t>& indices);
	IndexBuffer(uint32_t* indices, uint32_t count);
};