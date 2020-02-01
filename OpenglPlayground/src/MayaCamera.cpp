#include "MayaCamera.h"
#include "Renderer.h"
#include <iostream>

#define TWO_PI glm::two_pi<float>()

MayaCamera::MayaCamera(glm::vec3 position, float FOV)
{
	m_viewMat = glm::mat4(1.0f);

	m_orientation = glm::quat(0.0f,0.0f,0.0f,-1.0f);//w, (x,y,z)
	
	m_eyePosition = position;
	m_zoom = FOV;
	m_focalPoint = glm::vec3(0.0f);//origin

	m_distance = glm::distance(m_eyePosition, m_focalPoint);

	m_rotationSpeed = 2.0f;
	m_zoomSpeed = 3.0f;
	m_cameraSpeed = 5.0f;
	m_panSpeed = 0.0090f;
	
	m_pitch = 0.0f;
	m_yaw = 0.0f;
	m_roll = 0.0f;
	
	m_zoomOffset = 0.0f;
	
	m_mousePosition = glm::vec2(float(WIDTH) / 2.0f, float(HEIGHT) / 2.0f);
	UpdateVectors();
}

MayaCamera::~MayaCamera()
{
}

void MayaCamera::UpdateCamera(GLFWwindow* win, float deltaTime)
{
	KeyboardMovement(win, deltaTime);

	if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		GLCALL(glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED));
		double mx, my;
		GetMousePosition(win, mx, my);
		glm::vec2 mouseCurrent(mx, my);
		glm::vec2 mouseDelta(mx - m_mousePosition.x, my - m_mousePosition.y);
		m_mousePosition = mouseCurrent;
		//rotating effect
		if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			MouseRotating(mouseDelta, deltaTime);
		//zoom effect 
		if (glfwGetKey(win, GLFW_KEY_Z) == GLFW_PRESS)
			m_zoomOffset -= 1.0f;
		else if (glfwGetKey(win, GLFW_KEY_X) == GLFW_PRESS)
			m_zoomOffset += 1.0f;
		
		CameraZooming(m_zoomOffset, deltaTime);
		//panning effect
		if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		MousePanning(mouseDelta, deltaTime);
	}
	else
	{
		GLCALL(glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL));
	}

	
}
glm::mat4 MayaCamera::GetViewMatrix(void)
{
	glm::quat inverseOrientation = glm::conjugate(m_orientation);

	glm::mat4 rotation = glm::mat4_cast(inverseOrientation);
	//adding the traslation mannually(more efficient)
	rotation[3][0] = -(rotation[0][0] * m_eyePosition.x + rotation[1][0] * m_eyePosition.y + rotation[2][0] * m_eyePosition.z);
	rotation[3][1] = -(rotation[0][1] * m_eyePosition.x + rotation[1][1] * m_eyePosition.y + rotation[2][1] * m_eyePosition.z);
	rotation[3][2] = -(rotation[0][2] * m_eyePosition.x + rotation[1][2] * m_eyePosition.y + rotation[2][2] * m_eyePosition.z);
	rotation[3][3] = 1;

	//Is the same of doing this and multiply times rotation
	//glm::mat4 traslation = glm::translate(glm::mat4(1.0f), -m_eyePosition);
	
	return rotation;
}
void MayaCamera::GetMousePosition(GLFWwindow* win, double&x, double& y)
{
	GLCALL(glfwGetCursorPos(win, &x, &y));
}

void MayaCamera::KeyboardMovement(GLFWwindow* win, float dt)
{
	 float speed = m_cameraSpeed * dt;
	glm::quat qF = m_orientation * glm::quat(0.0f, 0.0f, 0.0f, -1.0f) * glm::conjugate(m_orientation);
	glm::vec3 frontDir = { qF.x, qF.y, qF.z };
	glm::vec3 upDir = { 0.0f,1.0f,0.0f };
	glm::vec3 rightDir = glm::normalize(glm::cross(frontDir, upDir));
	
	if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS)
		m_eyePosition -= upDir * speed;
	else if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS)
		m_eyePosition += upDir * speed;

	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
		m_eyePosition += frontDir * speed;
	else if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
		m_eyePosition -= frontDir * speed;

	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
		m_eyePosition -= rightDir * speed;
	else if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
		m_eyePosition += rightDir * speed;

	//update distance to target
	m_distance = glm::distance(m_eyePosition, m_focalPoint);
}

void MayaCamera::MouseRotating(glm::vec2 delta, float dt)
{
	float speed = m_rotationSpeed * dt;

	const float xOffset = delta.x * speed;
	const float yOffset = delta.y * speed;
	
	m_pitch += xOffset;
	m_yaw += yOffset;

	//limits to yaw to a rotation
	if (glm::radians(m_yaw) > TWO_PI) m_yaw -= TWO_PI;
	else if (glm::radians(m_yaw) < -TWO_PI) m_yaw += TWO_PI;


	//return the pitch to zero
	if (glm::radians(m_pitch) > TWO_PI) m_pitch = TWO_PI;
	else if (glm::radians(m_pitch) < -TWO_PI) m_pitch = TWO_PI;
	
	UpdateVectors();
}


void MayaCamera::CameraZooming(float yOffset, float dt)
{	
	const float speed = (m_zoomSpeed * m_zoomSpeed) * dt;
	
	if (m_zoom >= 1.0f && m_zoom <= 90.0f)
		m_zoom -= yOffset * speed;
	if (m_zoom < 1.0f)
		m_zoom = 1.0f;
	if (m_zoom > 90.0f)
		m_zoom = 90.0f;

	m_distance -= yOffset * speed;
	if (m_distance < 1.0f)
	{
		glm::quat qF = m_orientation * glm::quat(0.0f, 0.0f, 0.0f, -1.0f) * glm::conjugate(m_orientation);
		glm::vec3 frontDir = { qF.x, qF.y, qF.z };
		m_focalPoint += frontDir;
		m_distance = 1.0f;
	}
	
	m_zoomOffset = 0.0f;
}

void MayaCamera::MousePanning(const glm::vec2 delta, float dt)
{
	glm::quat qF = m_orientation * glm::quat(0.0f, 0.0f, 0.0f, -1.0f) * glm::conjugate(m_orientation);
	glm::vec3 frontDir = { qF.x, qF.y, qF.z };
	glm::vec3 upDir = { 0.0f,1.0f,0.0f };
	glm::vec3 rightDir = glm::normalize(glm::cross(frontDir, upDir));
	const float speed = m_panSpeed * dt;
	m_focalPoint += -rightDir * speed * m_distance * delta.x;
	m_focalPoint += upDir * speed * m_distance * delta.y;

	//update position
	m_eyePosition = m_focalPoint - frontDir * m_distance;
	m_distance = glm::distance(m_eyePosition, m_focalPoint);
}

void MayaCamera::UpdateVectors(void)
{
	glm::quat yawQuat = Rotate(-m_pitch, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat pitchQuat = Rotate(m_yaw, glm::vec3(1.0f, 0.0f, 0.0f));
	//glm::quat rollQuat = Rotate(m_roll, glm::vec3(0.0f, 0.0f, 1.0f));

	m_orientation =  yawQuat * pitchQuat;
}


glm::quat MayaCamera::Rotate(float angle, const glm::vec3 axis)
{
	return glm::angleAxis(glm::radians(angle), axis);
}
