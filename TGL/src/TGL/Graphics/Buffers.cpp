#include "tglpch.h"
#include "Buffers.h"

Buffer::Buffer(GLenum type, const void* data, uint32_t size, uint32_t count, GLenum usage)
	: m_BufferType(type), m_Size(size), m_Count(count), m_BufferUsage(usage) {
	glGenBuffers(1, &m_ID);
	glBindBuffer(m_BufferType, m_ID);
	glBufferData(m_BufferType, m_Size, data, m_BufferUsage); 
}

Buffer::~Buffer(){
	glDeleteBuffers(1, &m_ID);
}

void Buffer::SetData(const void* data, uint32_t size){
	glBindBuffer(m_BufferType, m_ID);
	glBufferSubData(m_BufferType, 0, size, data);
}

Ref<VertexBuffer> VertexBuffer::Create(const std::vector<float>& vertices) {
	return CreateRef<VertexBuffer>(vertices);
}

Ref<VertexBuffer> VertexBuffer::Create(const uint32_t size) {
	return CreateRef<VertexBuffer>(size);
}

VertexBuffer::VertexBuffer(const std::vector<float>& vertices)
	: Buffer(GL_ARRAY_BUFFER, &vertices[0], sizeof(float) * vertices.size(), vertices.size()), m_Stride(0) {

}

VertexBuffer::VertexBuffer(const uint32_t size) 
	: Buffer(GL_ARRAY_BUFFER, nullptr, size, 0, GL_DYNAMIC_DRAW), m_Stride(0) {

}

void VertexBuffer::AddAttrib(uint32_t count, GLenum type, bool normalized){
	m_Attribs.push_back({
		(uint32_t)m_Attribs.size(),
		count, 
		type, 
		normalized,  
		m_Stride
	});
	m_Stride += count * GetTypeSize(type);
}

Ref<IndexBuffer> IndexBuffer::Create(const std::vector<uint32_t>& indices) {
	return CreateRef<IndexBuffer>(indices);
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
	return CreateRef<IndexBuffer>(indices, count);
}

IndexBuffer::IndexBuffer(const std::vector<uint32_t>& indices)
	: Buffer(GL_ELEMENT_ARRAY_BUFFER, &indices[0], sizeof(uint32_t)* indices.size(), indices.size()) {

}

IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
	: Buffer(GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(uint32_t) * count, count) {

}

uint32_t GetTypeSize(GLenum type) {
	switch (type) {
	case GL_FLOAT:
		return sizeof(float);
	case GL_UNSIGNED_INT:
		return sizeof(uint32_t);
	case GL_INT:
		return sizeof(int);
	}

	ASSERT(false, "GetTypeSize: Type {0} not supported", type);
}
