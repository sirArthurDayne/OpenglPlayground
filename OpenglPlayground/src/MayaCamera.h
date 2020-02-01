#pragma once
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "GL/glew.h"
#include"GLFW/glfw3.h"

class MayaCamera
{
public:
	enum class MOVE_STATES{IDLE, UP, DOWN, LEFT, RIGHT, FORWARD, BACKWARD};
	MayaCamera(glm::vec3 position, float FOV);
	~MayaCamera();

	void UpdateCamera(GLFWwindow* win, float deltaTime);

	
	glm::mat4 GetViewMatrix(void);
	glm::vec3 GetEyePosition(void) const { return m_eyePosition; }
	glm::vec2 GetPY(void) const { return glm::vec2(m_pitch, m_yaw); }
	float GetCameraFOV(void) const { return m_zoom; };
	float GetDistance(void) const { return m_distance; }
	
	void SetCameraFOV(float zoom) { m_zoom = zoom; }
private:
	void UpdateVectors(void);

	void GetMousePosition(GLFWwindow* win, double& x, double& y);

	void KeyboardMovement(GLFWwindow* win, float dt);

	void CameraZooming(float yOffset, float dt);
	
	void MousePanning(const glm::vec2 delta, float dt);

	void MouseRotating(glm::vec2 delta, float dt);

	glm::quat Rotate(float angle, const glm::vec3 axis);

private:
	glm::mat4 m_viewMat;

	glm::quat m_orientation;
	
	glm::vec3 m_eyePosition,  m_focalPoint;
	
	float m_pitch, m_yaw, m_roll;

	float m_distance;

	float m_cameraSpeed, m_rotationSpeed, m_zoomSpeed, m_panSpeed;

	float m_zoom;
	float m_zoomOffset;
	
	glm::vec2 m_mousePosition;
};


