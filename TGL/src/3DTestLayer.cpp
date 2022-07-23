#include "tglpch.h"
#include "3DTestLayer.h"


L3DTestLayer::L3DTestLayer() : Layer("3D Tests") {
	m_Cam = CreateRef<Camera3D>();
	m_Cam->SetPosition(glm::vec3(3.0f, 2.0f, 3.0f));
	//m_Cam->SetTarget({ -3.8f, -2.0f, -12.3f });

	m_Light = CreateRef<Light>(Light::Type::POINT);
	m_Light->SetAmbient({ .9f, .9f, .9f });
	m_Light->SetDiffuse({ .8f, .8f, .8f });
	m_Light->SetSpecular({ 1.0f, 1.0f, 1.0f });

	m_Light->SetPosition({ .0f, 5.0f, .0f });
	m_Light->SetAttenuation(1.0f, 0.09f, 0.032f);

	m_Light->SetCutOff(12.5f);
	m_Light->SetOuterCutOff(17.5f);
}

L3DTestLayer::~L3DTestLayer() {

}

void L3DTestLayer::OnAttach() {
	Registry::SetTexturePathPrefix("assets/textures/");
	Registry::SetModelsPathPrefix("assets/models/");

	// Box
	m_Material = CreateRef<Material>();
	m_Material->SetSpecular({ 0.5f, .5f, .5f });
	m_Material->SetShininess(.5f);
	m_Material->SetAmbientMap(Registry::GetTexture("container.png"));
	m_Material->SetSpecularMap(Registry::GetTexture("container_specular.png"));

	// Load Model
	//Registry::GetModel("street_scene.obj");
	Registry::GetModel("cube.obj");
}



void L3DTestLayer::OnUpdate(float ts) {
	if (Keyboard::IsReleased(GLFW_KEY_LEFT_ALT))
		Application::GetInstance()->GetWindow()->ToggleCursor();

	//m_Cam->OnUpdate(ts);
}

void L3DTestLayer::OnDraw() {
	Renderer::Begin(Renderer::R3D, m_Cam);

	Renderer::AddLight(m_Light);

	//Renderer3D::DrawModel(Registry::GetModel("street_scene.obj"), { 10.0f, .0f, 10.0f });
	Renderer3D::DrawModel(Registry::GetModel("cube.obj"), { .0f, .0f, .0f }, glm::vec4(1.0f));

	// cube
	Renderer3D::DrawCube(glm::vec3(0.0f, 0.0f, 0.0f), m_Material, glm::vec3(.5f));

	// light
	//Renderer3D::DrawCube(m_Light->GetPosition(), glm::vec3(0.05f));

	Renderer::End();
}