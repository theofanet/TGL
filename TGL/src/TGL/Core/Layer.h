#pragma once

#include "Core.h"
#include "Events.h"
#include <vector>


class Layer {
public:
	Layer(const std::string& name = "unnamed");
	virtual ~Layer();

	virtual void OnAttach() {};
	virtual void OnDettach() {};
	virtual void OnUpdate(float ts) {};
	virtual void OnDraw() {};
	virtual void OnGuiDraw() {};

	virtual bool OnEvent(Event& e) { return true; }
protected:
	std::string m_Name;
};

class ImGuiLayer : public Layer {
public:
	ImGuiLayer(const std::string& name = "Gui") : Layer(name) {}
};


class LayerStack {
public:
	LayerStack();
	~LayerStack();

	void PushLayer(Ref<Layer> layer);
	void PushOverlay(Ref<Layer> overlay);
	void PopLayer(Ref<Layer> layer);
	void PopOverlay(Ref<Layer> overlay);

	std::vector<Ref<Layer>>::iterator begin() { return m_Layers.begin(); }
	std::vector<Ref<Layer>>::iterator end() { return m_Layers.end(); }

private:
	std::vector<Ref<Layer>> m_Layers;
	unsigned int m_LayerInsertIndex = 0;
};