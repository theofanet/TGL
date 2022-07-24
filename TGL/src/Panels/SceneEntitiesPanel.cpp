#include "tglpch.h"
#include "SceneEntitiesPanel.h"

#include "imgui.h"
#include "TGL/Scene/Components.h"


SceneEntitiesPanel::SceneEntitiesPanel(const Ref<Scene>& scene){
	SetContext(scene);
}

void SceneEntitiesPanel::SetContext(const Ref<Scene>& scene){
	m_Context = scene;
}

void SceneEntitiesPanel::OnImGuiRender(){
	ImGui::Begin("Entities");

	m_Context->m_Registry.each([&](auto entityID) {
		Entity entity{ entityID, m_Context.get() };
		DrawEntityNode(entity);
	});

	ImGui::End();
}

void SceneEntitiesPanel::DrawEntityNode(Entity e){
	auto& tag = e.GetComponent<TagComponent>().Tag;

	ImGuiTreeNodeFlags flags = (m_SelectionContext == e ? ImGuiTreeNodeFlags_Selected : 0) 
		| ImGuiTreeNodeFlags_OpenOnArrow;

	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)e, flags, tag.c_str());

	if (ImGui::IsItemClicked())
		m_SelectionContext = e;

	if (opened)
		ImGui::TreePop();
}
