#include "tglpch.h"
#include "ContentBrowserPanel.h"

#include "TGL/Core/Registry.h"

#include "imgui.h"


ContentBrowserPanel::ContentBrowserPanel()
	: m_CurrentDirectory("assets") {
	m_FileIcon = Registry::GetTexture("assets/editor/icons/FileIcon.png");
	m_DirectoryIcon = Registry::GetTexture("assets/editor/icons/DirectoryIcon.png");
}

void ContentBrowserPanel::OnImGuiRender(){
	ImGui::Begin("Content Browser");

	if (m_CurrentDirectory != std::filesystem::path("assets")) {
		if (ImGui::Button("<-"))
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
	}

	static float padding = 16.0f;
	static float thumbnailSize = 64.0f;
	float cellSize = thumbnailSize + padding*2;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
		const auto& path = directoryEntry.path();
		std::string filenameStr = path.filename().string();

		ImGui::PushID(filenameStr.c_str());
		Ref<Texture> icn = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::ImageButton((ImTextureID)icn->GetID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

		if (ImGui::BeginDragDropSource()) {
			auto relativePath = std::filesystem::relative(path, "assets");
			const wchar_t* itemPath = relativePath.c_str();
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
			ImGui::EndDragDropSource();
		}

		ImGui::PopStyleColor();

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			if (directoryEntry.is_directory())
				m_CurrentDirectory /= path.filename();
		}
		
		ImVec2 textSize = ImGui::CalcTextSize(filenameStr.c_str());
		ImGui::SetCursorPosX(cellSize * ImGui::GetColumnIndex() + cellSize * 0.5 - textSize.x * 0.5);
		ImGui::TextWrapped(filenameStr.c_str());

		ImGui::NextColumn();

		ImGui::PopID();
	}

	ImGui::Columns(1);

	ImGui::End();
}


