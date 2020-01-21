#include "FPSCamera.h"
#include "Renderer.h"

#define PI glm::pi<float>()

FPSCamera::FPSCamera(glm::vec3 position, float FOV)
	: m_eyePosition(position), m_FOV(FOV)
{
	m_viewMatrix = glm::mat4(1.0f);

	m_mouseMiddlePosition = glm::vec2(float(WIDTH)/2.0f,float(HEIGHT)/2.0f);
	m_mouseSpeed = 2.50f;
	m_moveSpeed = 4.0f;

	m_pitch = 0.0f;
	m_yaw = 0.0f;

	m_orientation = GetOrientation();
	UpdateViewMatrix();
}

FPSCamera::~FPSCamera()
{
}

void FPSCamera::UpdateCamera(GLFWwindow* win, float deltaTime)
{
	if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		GLCALL(glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED));
		double x, y;
		GLCALL(glfwGetCursorPos(win,&x, &y));
		glm::vec2 currentMousePos(x,y);
		currentMousePos -= m_mouseMiddlePosition;

		UpdateViewRotations(currentMousePos, deltaTime);
		GLCALL(glfwSetCursorPos(win, m_mouseMiddlePosition.x, m_mouseMiddlePosition.y));
		m_orientation = GetOrientation();
		KeyboardMovement(win, deltaTime);

		UpdateViewMatrix();
	}
	
}

void FPSCamera::KeyboardMovement(GLFWwindow* win, float dt)
{
		glm::quat qF = m_orientation * glm::quat(0.0f, 0.0f, 0.0f, -1.0f) * glm::conjugate(m_orientation);
		glm::vec3 frontDir = glm::vec3(qF.x, qF.y, qF.z);
		glm::vec3 upDir = { 0.0f, 1.0f, 0.0f };
		glm::vec3 rightDir = glm::normalize(glm::cross(frontDir, upDir));

		const float speed = glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? (m_moveSpeed * 2.0f) * dt : m_moveSpeed * dt;
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
		else	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
			m_eyePosition += rightDir * speed;
}

void FPSCamera::UpdateViewRotations(glm::vec2 delta, float dt)
{
	const float speed = m_mouseSpeed * dt;
	m_yaw   += delta.x * speed;
	m_pitch += delta.y * speed;

	if (glm::radians(m_pitch) > PI/4.0f) m_pitch -= PI/4.0f;//mirar abajo
	else if (glm::radians(m_pitch) < -PI/2.0f) m_pitch += PI/2.0f;//mirar arriba
}

void FPSCamera::UpdateViewMatrix(void)
{
	//Update View Matrix
	glm::quat inverseOrientation = glm::conjugate(m_orientation);
	glm::mat4 rotation = glm::mat4_cast(inverseOrientation);
	glm::mat4 traslation = glm::translate(glm::mat4(1.0f), -m_eyePosition);

	m_viewMatrix = rotation * traslation;
}

glm::quat FPSCamera::GetOrientation(void)
{
	glm::quat yawQuat = Rotate(-m_yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat pitchQuat = Rotate(-m_pitch, glm::vec3(1.0f, 0.0f, 0.0f));

	return yawQuat * pitchQuat;
}

glm::quat FPSCamera::Rotate(float angle, glm::vec3 axis)
{
	return glm::angleAxis(glm::radians(angle), axis);
}


