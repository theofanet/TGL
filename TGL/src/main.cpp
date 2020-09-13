#include "tglpch.h"
#include "TGL.h"

#include "Physics/2D/World.h"

#include <glad/glad.h>

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};


class S3DLayer : public Layer{
public:
	S3DLayer() : Layer("3d Test"), m_Rot(.0f) {
		m_Cam = CreateRef<Camera3D>(glm::vec3(3.0f, 0.0f, 3.0f));
		m_Cam->SetTarget({ -3.8f, -2.0f, -12.3f });

		m_Light = CreateRef<Light>(Light::Type::FLASH);
		m_Light->SetAmbient({ .03f, .03f, .03f });
		m_Light->SetDiffuse({ .8f, .8f, .8f });
		m_Light->SetSpecular({ 1.0f, 1.0f, 1.0f });

		// For DIRECTIONAL & FLASHLIGHT type
		//m_Light->SetDirection({ -0.2f, -1.0f, -0.3f });
		m_Light->SetDirection({ -3.8f, -2.0f, -12.3f });

		// For POINT & FLASHLIGHT type
		//m_Light->SetPosition({ 1.2f, 1.0f, 2.0f });
		m_Light->SetPosition({ 3.0f, 0.0f, 3.0f });
		m_Light->SetAttenuation(1.0f, 0.09f, 0.032f);

		// For FLASHLIGHT
		m_Light->SetCutOff(12.5f);
		m_Light->SetOuterCutOff(17.5f);
	}

	virtual void OnAttach() {
		Registry::SetTexturePathPrefix("assets/textures/");

		// Box
		m_Material = CreateRef<Material>();
		m_Material->SetSpecular({ 0.5f, .5f, .5f });
		m_Material->SetShininess(.5f);
		m_Material->SetAmbientMap(Registry::GetTexture("container.png"));
		m_Material->SetSpecularMap(Registry::GetTexture("container_specular.png"));
	}

	virtual void OnUpdate(float ts) {
		m_Rot += .5f * ts;

		if (Keyboard::IsReleased(GLFW_KEY_D))
			m_Light->SetType(Light::Type::DIRECTIONAL);
		if (Keyboard::IsReleased(GLFW_KEY_P))
			m_Light->SetType(Light::Type::POINT);
		if (Keyboard::IsReleased(GLFW_KEY_F))
			m_Light->SetType(Light::Type::FLASH);
	}

	virtual inline void OnDraw() {
		Renderer::Begin(Renderer::R3D, m_Cam);

		Renderer::AddLight(m_Light);

		// cube
		for (unsigned int i = 0; i < 10; i++)
			Renderer3D::DrawCube(cubePositions[i], m_Material, glm::vec3(.5f), m_Rot);
		
		// light
		Renderer3D::DrawCube(m_Light->GetPosition(), glm::vec3(0.05f));

		Renderer::End();
	}

private:
	Ref<Light> m_Light;
	Ref<Camera3D> m_Cam;
	Ref<Material> m_Material;
	float m_Rot;
};


class TestLayer : public Layer {
public:
	TestLayer() : Layer("Test layer"), m_Speed(1.0f), m_Margin(0.0) {
		m_Cam = CreateRef<Camera2D>(800 / 600);
		m_MouseDown = false;
	}

	virtual void OnAttach() {
		Registry::SetTexturePathPrefix("assets/textures/");
		SUB_EVENT(EventMouseScroll, TestLayer::OnScroll);
		m_MousePosition = Mouse::GetPosition();

		m_World.CreateBody(Body2D::DYNAMIC, glm::vec2(0.0, 1.0), glm::vec2(1.0f));
	}

	virtual inline void OnUpdate(float ts) {
		if (Keyboard::IsPressed(GLFW_KEY_ESCAPE))
			Application::Quit();

		if (Keyboard::IsHeld(GLFW_KEY_LEFT_SHIFT))
			m_Speed = 2.0;
		else
			m_Speed = 1.0f;

		if (Keyboard::IsHeld(GLFW_KEY_M))
			m_Margin = .01f;
		else
			m_Margin = .0f;

		glm::vec3 pos = m_Cam->GetPosition();
		if (Keyboard::IsHeld(GLFW_KEY_W))
			pos.y += m_Speed * ts;
		else if (Keyboard::IsHeld(GLFW_KEY_S))
			pos.y -= m_Speed * ts;

		if (Keyboard::IsHeld(GLFW_KEY_A))
			pos.x -= m_Speed * ts;
		else if (Keyboard::IsHeld(GLFW_KEY_D))
			pos.x += m_Speed * ts;

		if(pos != m_Cam->GetPosition())
			m_Cam->SetPosition(pos);

		std::ostringstream title;
		title << Application::GetInstance()->GetWindowProps().Title << " - FPS : " << (60 / ts);
		Application::GetInstance()->GetWindow()->SetTitle(title.str());

		if (Mouse::IsButtonPress(MOUSE_LEFT_BUTTON))
			INFO("MOUSE BUTTON LEFT PRESS");
		if (Mouse::IsButtonRelease(MOUSE_LEFT_BUTTON))
			INFO("MOUSE BUTTON LEFT RELEASE");
		if (Mouse::IsButtonHeld(MOUSE_LEFT_BUTTON)) {
			m_MouseDown = true;
			INFO("MOUSE BUTTON LEFT HELD");
		}
		else
			m_MouseDown = false;
	
		if (m_MouseDown) {
			glm::vec2 pos = Mouse::GetPosition() - m_MousePosition;
			m_Cam->SetPosition(m_Cam->GetPosition() + glm::vec3(-pos.x / 600.0, pos.y / 400.0f, 0.0f) * m_Cam->GetZoomLevel());
		}
		m_MousePosition = Mouse::GetPosition();

		m_World.Update(ts);
	}

	virtual inline void OnDraw() {
		Renderer::Begin(Renderer::R2D, m_Cam);

		for (int y = 0; y < 40; y++) {
			for (int x = 0; x < 40; x++) {
				int i = x * y + x;
				Renderer2D::DrawQuad(
					"grass.png",
					{ x * (0.1 + m_Margin), y * (0.1 + m_Margin) },
					{ 0.1, 0.1 }
				);

				if (i % 17 == 0) {
					Renderer2D::DrawQuad(
						"glass05red.png",
						{ x * (0.1 + m_Margin), y * (0.1 + m_Margin) },
						{ 0.1, 0.1 }
					);
				}
			}
		}

		glm::vec4 color = {0.0f, 0.0f, 1.0f, 1.0f};
		for (int y = 0; y < 40; y++) {
			color.r += 0.025;
			color.b -= 0.025;

			for (int x = -1; x > -41; x--) {
				color.g += 0.025;

				Renderer2D::DrawQuad(
					{ x * (0.1 + m_Margin), y * (0.1 + m_Margin) },
					{ 0.1, 0.1 },
					color
				);
			}
			color.g = 0;
		}

		Renderer::End();

		m_World.DrawDebug(m_Cam);
	}

	inline bool OnScroll(EventMouseScroll& e) {
		float z = m_Cam->GetZoomLevel();
		z -= e.Y * 0.25;
		z = std::max(z, 0.25f);
		m_Cam->SetZoomLevel(z);
		return true;
	}

private:
	Ref<Camera2D> m_Cam;
	float m_Speed;
	float m_Margin;
	bool m_MouseDown;
	glm::vec2 m_MousePosition;

	World2D m_World;
};


int main(int argc, char* argv[]) {
	Application app(WindowProps("Test TGL", 800, 600));
	app.PushLayer(CreateRef<S3DLayer>());
	app.Run();

	return 0;
}