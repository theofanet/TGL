#pragma once
#pragma once

#include "TGL/TGL.h"

#include "Panels/SceneEntitiesPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/StatsInfoPanel.h"


class MyLayer : public Layer {
public:
	MyLayer();
	~MyLayer();

	virtual void OnAttach();
	virtual void OnUpdate(float ts);
	virtual void OnDraw();
	virtual void OnGuiDraw();

private:
	Ref<Scene> m_ActiveScene;

	EditorCamera m_EditorCamera;

	SceneEntitiesPanel m_EntitiesPanel;
	ContentBrowserPanel m_ContentBrowserPanel;
	ViewportPanel m_ViewportPanel;
	StatsInfoPanel m_StatsPanel;
};