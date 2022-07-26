#include "tglpch.h"
#include "EntityPropertyPanel.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "TGL/Scene/Components.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>



EntityPropertyPanel::EntityPropertyPanel(const Ref<Scene>&scene) 
	: m_Context(scene) {

}

void EntityPropertyPanel::SetContext(const Ref<Scene>& scene){
	m_Context = scene;
}

void EntityPropertyPanel::OnImGuiRender(const Entity& entity){
	ImGui::Begin("Property");

	ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImGui::Separator();
	ImGui::Text("Components");
	ImGui::PopStyleVar();
	ImGui::SameLine(contentRegionAvailable.x - 4);
	if (ImGui::Button("+"))
		ImGui::OpenPopup("AddComponent");
	DrawAddComponentMenu(entity, "AddComponent");

	DrawComponent<TagComponent>("Tag", entity, [](auto& component) {
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), component.Tag.c_str());

		if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			component.Tag = std::string(buffer);
	});

	DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
		ImGui::DragFloat3("Translation", glm::value_ptr(component.Translation));

		glm::vec3 rotation = glm::degrees(component.Rotation);
		if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation)))
			component.Rotation = glm::radians(rotation);

		ImGui::DragFloat3("Scale", glm::value_ptr(component.Scale));
	});

	DrawComponent<SpriteRendererComponent>("Sprite", entity, [](auto& component) {
		ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

		ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path texturePath = std::filesystem::path("assets") / path;
				Ref<Texture> texture = Registry::GetTexture(texturePath.string());
				component.Texture = texture;
			}
			ImGui::EndDragDropTarget();
		}
	});

	ImGui::End();
}

void EntityPropertyPanel::DrawAddComponentMenu(const Entity& entity, const std::string& name) {
	if (ImGui::BeginPopup(name.c_str())) {
		DrawAddComponentMenuItem<CameraComponent>("Add Camera", entity);
		DrawAddComponentMenuItem<TransformComponent>("Add Transform", entity);
		DrawAddComponentMenuItem<SpriteRendererComponent>("Add Sprite", entity);
		ImGui::EndPopup();
	}
}

template<typename T, typename UIFun>
static void EntityPropertyPanel::DrawComponent(const std::string& name, const Entity& entity, UIFun uiFunction) {
	if (entity.HasComponent<T>()) {
		auto& component = entity.GetComponent<T>();
		bool removeComponent = false;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
		ImGui::PopStyleVar();

		if (!std::is_same<T, TagComponent>::value) {
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			ImGui::PushID(name.c_str());
			if (ImGui::Button("X", ImVec2{ lineHeight, lineHeight }))
				removeComponent = true;
			ImGui::PopID();
		}

		if (opened) {
			uiFunction(component);
			ImGui::TreePop();
		}

		if (removeComponent) 
			entity.RemoveComponent<T>();
	}
}

template<typename T>
static void EntityPropertyPanel::DrawAddComponentMenuItem(const std::string& name, const Entity& entity) {
	if (!entity.HasComponent<T>()) {
		if (ImGui::MenuItem(name.c_str()))
			entity.AddComponent<T>();
	}
}