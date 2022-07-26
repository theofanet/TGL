#include "tglpch.h"
#include "Scene.h"

#include "Components.h"
#include "TGL/Graphics/Renderer2D.h"
#include "Entity.h"
#include "ScriptableEntity.h"


Scene::Scene() {

}

Scene::~Scene() {

}

Entity Scene::CreateEntity(const std::string& name){
	Entity entity(m_Registry.create(), this);
	entity.AddComponent<TransformComponent>();
	entity.AddComponent<TagComponent>(name);
	return entity;
}

void Scene::DestroyEntity(const Entity& entity){
	m_Registry.destroy(entity);
}

void Scene::OnUpdate(float ts){
	m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
		if (!nsc.Instance) {
			nsc.Instance = nsc.InstantiateScript();
			nsc.Instance->m_Entity = Entity{ entity, this };
			nsc.Instance->OnCreate();
		}
		
		nsc.Instance->OnUpdate(ts);
	});
}

void Scene::OnUpdateEditor(float ts){

}

void Scene::OnDraw(){
	SceneCamera* mainCamera = nullptr;
	glm::mat4* mainCameraTransform = nullptr;

	auto view = m_Registry.view<TransformComponent, CameraComponent>();
	for (auto entity : view) {
		auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
		if (camera.Primary) {
			mainCamera = &camera.Camera;
			mainCameraTransform = &transform.GetTransform();
		}
	}

	if (mainCamera) {
		Renderer2D::Begin(*mainCamera, *mainCameraTransform);
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) {
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
		}
		Renderer2D::End();
	}
}

void Scene::OnDrawEditor(const EditorCamera& camera){
	Renderer2D::Begin(camera);
	auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
	for (auto entity : group) {
		auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
		Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
	}
	Renderer2D::End();
}

void Scene::SetViewportSize(uint32_t width, uint32_t height){
	m_ViewportWidth = width;
	m_ViewportHeight = height;

	auto view = m_Registry.view<CameraComponent>();
	for (auto entity : view) {
		auto& camera = view.get<CameraComponent>(entity);
		if (!camera.FixedAspectRatio)
			camera.Camera.SetViewportSize(width, height);
	}
}
