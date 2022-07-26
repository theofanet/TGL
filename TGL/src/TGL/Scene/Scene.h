#pragma once

#include "entt.hpp"
#include "TGL/Core/Events.h"
#include "TGL/Graphics/EditorCamera.h"

class Entity;


class Scene {
public:
	Scene();
	~Scene();

	Entity CreateEntity(const std::string& name = "Entity");
	void DestroyEntity(const Entity& entity);

	entt::registry& Reg() { return m_Registry; }

	void OnUpdate(float ts);
	void OnUpdateEditor(float ts);
	void OnDraw();
	void OnDrawEditor(const EditorCamera& camera);

	void SetViewportSize(uint32_t width, uint32_t height);

private:
	entt::registry m_Registry;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	friend class Entity;
	friend class SceneEntitiesPanel;
};