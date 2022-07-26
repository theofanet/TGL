#pragma once

#include "Entity.h"


class ScriptableEntity {
public:
	virtual ~ScriptableEntity() {}

	template<typename T>
	T& GetComponent() {
		return m_Entity.GetComponent<T>();
	}

protected:
	virtual void OnCreate() {}
	virtual void OnDestroy() {}
	virtual void OnUpdate(float ts) {}
	virtual void OnDraw() {}
	virtual void OnEvent(Event& e) {}

private:
	Entity m_Entity;

	friend class Scene;
};