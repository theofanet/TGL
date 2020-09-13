#pragma once

#include "Core/Core.h"
#include "Graphics/Camera.h"
#include "Body.h"
#include <vector>


class World2D{
public:
	World2D();
	~World2D();

	Ref<Body2D> CreateBody(Body2D::Type type, const glm::vec2& position, const glm::vec2& size);

	void Update(float ts);
	void DrawDebug(Ref<Camera> cam);

private:
	std::vector<Ref<Body2D>> m_Bodies;
};