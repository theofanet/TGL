#pragma once

#include "Scene.h"
#include "entt.hpp"


class Entity {
public:
	Entity() = default;
	Entity(entt::entity e, Scene* scene)
		: m_Scene(scene), m_EntityID(e) { }
	Entity(const Entity& other) = default;

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args) const {
		ASSERT(!HasComponent<T>(), "Entity already have component");
		return m_Scene->m_Registry.emplace<T>(m_EntityID, std::forward<Args>(args)...);
	}

	template<typename T>
	T& GetComponent() const {
		ASSERT(HasComponent<T>(), "Entity dos not have component");
		return m_Scene->m_Registry.get<T>(m_EntityID);
	}

	template<typename T>
	bool HasComponent() const {
		return m_Scene->m_Registry.has<T>(m_EntityID);
	}

	template<typename T>
	void RemoveComponent() const {
		ASSERT(HasComponent<T>(), "Entity dos not have component");
		m_Scene->m_Registry.remove<T>(m_EntityID);
	}

	operator bool() const { return m_EntityID != entt::null; }
	operator uint32_t() const { return (uint32_t)m_EntityID; }
	operator entt::entity() const { return m_EntityID; }

	bool operator==(const Entity& other) const { return m_EntityID == other.m_EntityID && m_Scene == other.m_Scene; }
	bool operator!=(const Entity& other) const { return !operator==(other); }
protected:
	entt::entity m_EntityID = entt::null;
	Scene* m_Scene = nullptr;
};