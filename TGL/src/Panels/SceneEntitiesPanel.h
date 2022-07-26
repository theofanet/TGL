#pragma once

#include "TGL/Core/Core.h"
#include "TGL/Scene/Scene.h"
#include "TGL/Scene/Components.h"
#include "TGL/Scene/Entity.h"

#include "EntityPropertyPanel.h"

#include "EditorEvents.h"


class SceneEntitiesPanel {
public:
	SceneEntitiesPanel() {
		SUB_EVENT(EntitySelectedEvent, SceneEntitiesPanel::OnSelectedEntity);
	};
	SceneEntitiesPanel(const Ref<Scene>& scene);

	void SetContext(const Ref<Scene>& scene);

	void OnImGuiRender();

	inline Entity& GetSelectedEntity() { return m_SelectionContext; }
	inline void SetSelectedEntity(Entity& entity) { m_SelectionContext = entity; }

	bool OnSelectedEntity(EntitySelectedEvent& e);

private:
	void DrawEntityNode(Entity e);

private:
	Ref<Scene> m_Context;
	Entity m_SelectionContext;

	EntityPropertyPanel m_PropertyPanel;
};