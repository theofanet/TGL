#include "tglpch.h"
#include "Terrain.h"


Terrain::Terrain() {

}

Terrain::~Terrain() {

}

void Terrain::InitData(int width, int height) {
	m_Width = width;
	m_Height = height;

	std::vector<float> vertices;
	vertices.resize(((size_t)m_Width * (size_t)m_Height) * 6);
	int idx = 0;
	for (unsigned y = 0; y < m_Height; y++) {
		for (unsigned x = 0; x < m_Width; x++) {
			vertices[idx++] = (float)x;
			vertices[idx++] = rand() % 2; 
			vertices[idx++] = (float)y;
			vertices[idx++] = .0f;
			vertices[idx++] = 1.0f;
			vertices[idx++] = .0f;
		}
	}

	std::vector <uint32_t> indices;
	indices.resize(((size_t)m_Width - 1) * ((size_t)m_Height - 1) * 6);
	idx = 0;
	for (unsigned y = 0; y < (m_Height - 1); ++y) {
		for (unsigned x = 0; x < (m_Width - 1); ++x) {
			int start = y * m_Width + x;
			
			indices[idx++] = start;
			indices[idx++] = start + 1;
			indices[idx++] = start + m_Width;

			indices[idx++] = start + 1;
			indices[idx++] = start + 1 + m_Width;
			indices[idx++] = start + m_Width;
		}
	}

	Ref<VertexBuffer> vb = VertexBuffer::Create(vertices);
	vb->AddAttrib(3, GL_FLOAT); // x y z
	vb->AddAttrib(3, GL_FLOAT); // normals x y z

	Ref<IndexBuffer> ib = IndexBuffer::Create(indices);

	m_VA = VertexArray::Create();
	m_VA->AddVertexBuffer(vb);
	m_VA->SetIndexBuffer(ib);

	vb->Unbind();
	ib->Unbind();
	m_VA->Unbind();
}