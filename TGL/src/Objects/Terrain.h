#pragma once

#include "tglpch.h"
#include "TGL.h"


class Terrain {
public: 
	Terrain();
	~Terrain();

	void InitData(int width, int height);
	inline const Ref<VertexArray> GetVertexArray() { return m_VA; }

protected:
	int m_Width, m_Height;
	Ref<VertexArray> m_VA;
};