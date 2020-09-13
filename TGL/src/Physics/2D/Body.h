#pragma once

#include <glm/glm.hpp>
#include <vector>


class Body2D {
public:
	enum Type { DYNAMIC = 0, STATIC };

	Body2D(Type type, const glm::vec2& position, const glm::vec2& size);
	virtual ~Body2D();

	virtual void Update(float ts);

	inline virtual void AddForce(const glm::vec2& force) { m_Forces.push_back(force); }

	inline virtual void SetVelocity(const glm::vec2& velocity) { m_Velocity = velocity; }
	inline virtual void SetVelocity(float x, float y) { SetVelocity(glm::vec2(x, y)); }
	inline virtual void SetVelocityX(float x) { m_Velocity.x = x; }
	inline virtual void SetVelocityY(float y) { m_Velocity.y = y; }
	inline virtual const glm::vec2& GetVelocity() { return m_Velocity; }

	inline const glm::vec2& GetSize() { return m_Size; }
	inline const glm::vec2& GetPosition() { return m_Position; }

private:
	glm::vec2 m_Position;
	glm::vec2 m_Velocity;
	glm::vec2 m_Size;

	Type m_Type;

	std::vector<glm::vec2> m_Forces;
};