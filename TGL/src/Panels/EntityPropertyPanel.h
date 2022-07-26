#pragma once

#include "TGL/Core/Core.h"
#include "TGL/Scene/Scene.h"
#include "TGL/Scene/Components.h"
#include "TGL/Scene/Entity.h"


class EntityPropertyPanel {
public:
	EntityPropertyPanel() = default;
	EntityPropertyPanel(const Ref<Scene>& scene);

	void SetContext(const Ref<Scene>& scene);

	void OnImGuiRender(const Entity& entity);

private:
	template<typename T, typename UIFun>
	static void DrawComponent(const std::string& name, const Entity& entity, UIFun uiFunction);

	static void DrawAddComponentMenu(const Entity& entity, const std::string& name);

	template<typename T>
	static void DrawAddComponentMenuItem(const std::string& name, const Entity& entity);

private:
	Ref<Scene> m_Context;
};