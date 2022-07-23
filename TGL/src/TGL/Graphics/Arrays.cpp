#include "tglpch.h"
#include "Arrays.h"


Ref<VertexArray> VertexArray::Create(){
	return CreateRef<VertexArray>();
}

VertexArray::VertexArray() : m_IB(nullptr) {
	glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::Bind() {
	glBindVertexArray(m_ID);
}

void VertexArray::Unbind() {
	glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer) {
	Bind();
	buffer->Bind();
	for (const auto& a : buffer->GetAttribs()) {
		glEnableVertexAttribArray(a.index);
		glVertexAttribPointer(a.index, a.size, a.type, a.normalized ? GL_TRUE : GL_FALSE, buffer->GetStride(), (const void*)a.offset);
	}
	m_VBs.push_back(buffer);
}

void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer) {
	Bind();
	buffer->Bind();
	m_IB = buffer;
}

void VertexArray::Draw(uint32_t indexCount) {
	Bind();
	m_IB->Bind();
	uint32_t count = indexCount ? indexCount : m_IB->GetCount();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}
