#include "tglpch.h"
#include "StatsInfoPanel.h"

#include "TGL/Core/Application.h"
#include "TGL/Graphics/Renderer2D.h"

#include "imgui.h"


void StatsInfoPanel::OnImGuiRender(){
	ImGui::Begin("Stats");

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed
		| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

	bool opened = ImGui::TreeNodeEx("Graphics stats");
	
	if (opened) {
		ImGui::Text("FPS : %.2f", Application::GetFPS());
		ImGui::Text("Draw calls : %d", Renderer2D::s_StatsDrawCalls);
		ImGui::Text("Quads : %d", Renderer2D::s_StatsQuads);
		ImGui::Text("Vertices : %d", Renderer2D::s_StatsQuads * 4);
		ImGui::Text("Indices : %d", Renderer2D::s_StatsQuads * 6);

		ImGui::TreePop();
	}

	ImGui::End();
}
