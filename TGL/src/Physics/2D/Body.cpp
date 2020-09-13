#include "tglpch.h"
#include "Body.h"


Body2D::Body2D(Type type, const glm::vec2& position, const glm::vec2& size)
	: m_Type(type), m_Position(position), m_Size(size) {
	m_Velocity = glm::vec2(0.0f);
}

Body2D::~Body2D(){
}

void Body2D::Update(float ts){
	glm::vec2 acc = glm::vec2(0.0f);
	for (const auto& f : m_Forces)
		acc += f;
	m_Forces.clear();

	m_Velocity += acc * ts;
	m_Position += m_Velocity * ts;
}
