#pragma once

#include "Graphics/Arrays.h"
#include "Graphics/Material.h"

class Mesh {
public:
	Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, Ref<Material> material);
	~Mesh();

	inline Ref<VertexArray> GetVA() { return m_VA; }
	inline Ref<Material> GetMaterial() { return m_Material; }

protected:
	Ref<VertexArray> m_VA;
	Ref<Material> m_Material;
};