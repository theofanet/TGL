#pragma once

#include "TGL/Core/Core.h"
#include "TGL/Scene/Scene.h"
#include "TGL/Scene/Components.h"
#include "TGL/Scene/Entity.h"

#include "EntityPropertyPanel.h"


class SceneEntitiesPanel {
public:
	SceneEntitiesPanel() = default;
	SceneEntitiesPanel(const Ref<Scene>& scene);

	void SetContext(const Ref<Scene>& scene);

	void OnImGuiRender();

private:
	void DrawEntityNode(Entity e);

private:
	Ref<Scene> m_Context;
	Entity m_SelectionContext;

	EntityPropertyPanel m_PropertyPanel;
};