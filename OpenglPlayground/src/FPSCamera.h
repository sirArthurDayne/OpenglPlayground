#pragma once
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "GL/glew.h"
#include"GLFW/glfw3.h"

class FPSCamera
{
public:
	FPSCamera(glm::vec3 position, float FOV);
	~FPSCamera();

	void UpdateCamera(GLFWwindow* win, float deltaTime);

	
	glm::vec3 GetEyePosition(void) const { return m_eyePosition; }
	glm::vec2 GetPY(void) const { return glm::vec2(m_pitch, m_yaw); }
	glm::mat4 GetViewMatrix(void) const { return m_viewMatrix; }
	float GetCameraFOV(void) const { return m_FOV;}

	private:
	void KeyboardMovement(GLFWwindow* win, float dt);
	void UpdateViewRotations(glm::vec2 delta, float dt);
	void UpdateViewMatrix(void);
	
	glm::quat GetOrientation(void);
	glm::quat Rotate(float angle, glm::vec3 axis);
private:
	glm::vec3 m_eyePosition;
	glm::mat4 m_viewMatrix;
	glm::quat m_orientation;

	glm::vec2 m_mouseMiddlePosition;
	
	float m_mouseSpeed;
	float m_moveSpeed;
	//bool m_isMouseGrabbing;

	float m_pitch, m_yaw;

	float m_FOV;
};
