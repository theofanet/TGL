#pragma once

#include <glm/glm.hpp>


class Body2D {
public:
	Body2D();
	virtual ~Body2D();

	virtual void Update(float ts);

private:
	glm::vec2 m_Position;
	glm::vec2 m_Velocity;
	glm::vec2 m_Size;
};