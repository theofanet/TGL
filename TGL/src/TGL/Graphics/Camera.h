#pragma once

#include <glm/glm.hpp>
#include "TGL/Core/Events.h"


class RawCamera {
public:
	RawCamera() = default;
	RawCamera(const glm::mat4& projection) : m_Projection(projection) {}
	virtual ~RawCamera() = default;
	const glm::mat4& GetProjection() const { return m_Projection; }
protected:
	glm::mat4 m_Projection = glm::mat4(1.0f);
};


class Camera {
public:
	Camera(const glm::vec3& position = glm::vec3(0.0, 0.0, 0.0));
	virtual ~Camera();

	inline virtual const glm::vec3& GetPosition() { return m_Position; }
	inline virtual void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
	inline virtual void SetPosition(float x, float y, float z) { SetPosition({ x, y, z }); }
	inline virtual void SetPositionX(float x) { m_Position.x = x; RecalculateViewMatrix(); }
	inline virtual void SetPositionY(float y) { m_Position.y = y; RecalculateViewMatrix(); }
	inline virtual void SetPositionZ(float z) { m_Position.z = z; RecalculateViewMatrix(); }

	inline virtual const glm::vec3& GetRotation() { return m_Rotation; }
	inline virtual void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
	inline virtual void SetRotation(float x, float y, float z) { SetRotation({ x, y, z }); }
	inline virtual void SetRotationX(float x) { m_Rotation.x = x; RecalculateViewMatrix(); }
	inline virtual void SetRotationY(float y) { m_Rotation.y = y; RecalculateViewMatrix(); }
	inline virtual void SetRotationZ(float z) { m_Rotation.z = z; RecalculateViewMatrix(); }

	inline virtual void SetProjectionMatrix(const glm::mat4& matrix) { m_ProjectionMatrix = matrix; }

	inline virtual const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	inline virtual const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
	inline virtual const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

protected:
	virtual void RecalculateViewMatrix();

	glm::vec3 m_Position, m_Rotation;

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ViewProjectionMatrix;
};


class Camera2D : public Camera {
public:
	Camera2D(float aspectRatio, float zoomLevel = 1.0f);
	virtual ~Camera2D();

	void OnResize(float width, float height);

	void RecalculateProjection();
	void SetProjection(float left, float right, float bottom, float top);

	inline float GetZoomLevel() { return m_ZoomLevel; }
	inline void SetZoomLevel(float zoom) { m_ZoomLevel = zoom; RecalculateProjection(); }

protected:
	float m_AspectRatio, m_ZoomLevel;
};
