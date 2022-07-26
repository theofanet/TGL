#include "tglpch.h"
#include "SceneEntitiesPanel.h"

#include "imgui.h"
#include "TGL/Scene/Components.h"


SceneEntitiesPanel::SceneEntitiesPanel(const Ref<Scene>& scene)
	: m_PropertyPanel(scene) {
	SetContext(scene);
	SUB_EVENT(EntitySelectedEvent, SceneEntitiesPanel::OnSelectedEntity);
}

void SceneEntitiesPanel::SetContext(const Ref<Scene>& scene){
	m_Context = scene;
	m_PropertyPanel.SetContext(scene);
	m_SelectionContext = {};
}

void SceneEntitiesPanel::OnImGuiRender(){
	ImGui::Begin("Entities");

	if (m_Context) {
		m_Context->m_Registry.each([&](auto entityID) {
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);
		});

		if (ImGui::BeginPopupContextWindow(0, 1, false)){
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");
			ImGui::EndPopup();
		}
	}

	ImGui::End();

	if (m_SelectionContext)
		m_PropertyPanel.OnImGuiRender(m_SelectionContext);
}

bool SceneEntitiesPanel::OnSelectedEntity(EntitySelectedEvent& e){
	SetSelectedEntity(e.Ent);
	return true;
}

void SceneEntitiesPanel::DrawEntityNode(Entity e){
	auto& tag = e.GetComponent<TagComponent>().Tag;

	ImGuiTreeNodeFlags flags = (m_SelectionContext == e ? ImGuiTreeNodeFlags_Selected : 0) 
		| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)e, flags, tag.c_str());

	if (ImGui::IsItemClicked())
		m_SelectionContext = e;

	bool deleteEntity = false;
	if (ImGui::BeginPopupContextItem()){
		if (ImGui::MenuItem("Delete Entity"))
			deleteEntity = true;
		ImGui::EndPopup();
	}

	if (opened)
		ImGui::TreePop();

	if (deleteEntity) {
		m_Context->DestroyEntity(e);
		if (m_SelectionContext == e)
			m_SelectionContext = {};
	}
}
