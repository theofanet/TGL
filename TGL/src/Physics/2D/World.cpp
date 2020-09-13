#include "tglpch.h"
#include "World.h"

#include "Graphics/Renderer.h"
#include "Graphics/Renderer2D.h"


World2D::World2D(){
}

World2D::~World2D(){
}

Ref<Body2D> World2D::CreateBody(Body2D::Type type, const glm::vec2& position, const glm::vec2& size){
	Ref<Body2D> body = CreateRef<Body2D>(type, position, size);
	m_Bodies.push_back(body);
	return body;
}

void World2D::Update(float ts){
	for (const auto& b : m_Bodies) {
		b->AddForce(glm::vec2(0, -0.00981));
		b->Update(ts);
	}
}

void World2D::DrawDebug(Ref<Camera> cam) {
	Renderer::Begin(Renderer::R2D, cam);
	for (const auto& b : m_Bodies) {
		Renderer2D::DrawQuad(b->GetPosition(), b->GetSize(), glm::vec4(1.0, 0.3, 0.1, 1.0));
	}
	Renderer::End();
}