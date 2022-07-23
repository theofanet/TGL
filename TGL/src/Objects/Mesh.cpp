#include "tglpch.h"
#include "Mesh.h"


Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, Ref<Material> material)
	: m_Material(material) {
	Ref<VertexBuffer> vb = VertexBuffer::Create(vertices);
	vb->AddAttrib(3, GL_FLOAT); // x y z
	vb->AddAttrib(2, GL_FLOAT); // u v
	vb->AddAttrib(3, GL_FLOAT); // normals x y z

	Ref<IndexBuffer> ib = IndexBuffer::Create(indices);

	m_VA = VertexArray::Create();
	m_VA->AddVertexBuffer(vb);
	m_VA->SetIndexBuffer(ib);

	vb->Unbind();
	ib->Unbind();
	m_VA->Unbind();
}

Mesh::~Mesh() {

}