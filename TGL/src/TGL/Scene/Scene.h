#pragma once

#include "entt.hpp"

class Entity;


class Scene {
public:
	Scene();
	~Scene();

	Entity CreateEntity(const std::string& name = "Entity");

	entt::registry& Reg() { return m_Registry; }

	void OnUpdate(float ts);
	void OnDraw();

	void SetViewportSize(uint32_t width, uint32_t height);

private:
	entt::registry m_Registry;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	friend class Entity;
	friend class SceneEntitiesPanel;
};