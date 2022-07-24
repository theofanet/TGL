#pragma once

#include "TGL/TGL.h"
#include "Scene.h"
#include "entt.hpp"


class Entity {
public:
	Entity() = default;
	Entity(entt::entity e, Scene* scene)
		: m_Scene(scene), m_EntityID(e) { }
	Entity(const Entity& other) = default;

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args) {
		ASSERT(!HasComponent<T>(), "Entity already have component");
		return m_Scene->m_Registry.emplace<T>(m_EntityID, std::forward<Args>(args)...);
	}

	template<typename T>
	T& GetComponent() {
		ASSERT(HasComponent<T>(), "Entity dos not have component");
		return m_Scene->m_Registry.get<T>(m_EntityID);
	}

	template<typename T>
	bool HasComponent() {
		return m_Scene->m_Registry.has<T>(m_EntityID);
	}

	template<typename T>
	void RemoveComponent(){
		ASSERT(HasComponent<T>(), "Entity dos not have component");
		m_Scene->m_Registry.remove<T>(m_EntityID);
	}

	operator bool() const { return m_EntityID != entt::null; }

protected:
	entt::entity m_EntityID = entt::null;
	Scene* m_Scene = nullptr;
};