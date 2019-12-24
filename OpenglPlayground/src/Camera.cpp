#include "Camera.h"
#include "gtc/matrix_transform.hpp"


Camera::Camera(glm::vec3& cameraPos, glm::vec3 target, glm::vec3 up, glm::vec3 front, glm::vec3 initPoint):
	m_position(cameraPos), m_target(target),m_Up(up), m_rotationMat(glm::mat4(1.0f)),
	m_front(front), m_LastPointAtXY(initPoint)
{
	m_Right = glm::vec3(1.0f, 0.0, 0.0f);
	m_rotations = glm::vec3(0.0f);

}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::inverse(glm::translate(glm::mat4(1.0f), m_position + m_front) * m_rotationMat);
}

void Camera::UpdateCamera(Camera_Movement& move, float speed, float dt)
{
	Movement(move,speed, dt);
	//update camera view
	glm::vec3 cameraDirection = glm::normalize(m_position - m_target);
	m_Right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraDirection));
	m_Up = glm::cross(cameraDirection, m_Right);
	//setup rotations
	glm::mat4  rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotations.x), glm::vec3(1, 0, 0));
	glm::mat4  rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotations.y), glm::vec3(0, 1, 0));
	glm::mat4  rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotations.z), glm::vec3(0, 0, 1));
	m_rotationMat = rotationX * rotationY * rotationZ;
}

void Camera::Movement(Camera_Movement& move,float& cameraSpeed, float deltaTime)
{
	float speed = cameraSpeed * deltaTime;
	if (move == IDLE) speed = 0.0f;
	if (move == FORWARD) m_position += speed * m_front;
	else if (move == BACKWARD) m_position-= speed * m_front;
	if (move == LEFT) m_position-= m_Right * speed;
	else if (move == RIGHT) m_position+= m_Right  * speed;
}

void Camera::MouseMovement(const float x, const float y, const float z)
{
	//get distance from last frames & update
	float offsetX = m_LastPointAtXY.x -float(x);
	float offsetY = m_LastPointAtXY.y - float(y);
	m_LastPointAtXY.x = float(x);
	m_LastPointAtXY.y = float(y);

	const float sensitivity = 0.15f;
	offsetX *= sensitivity;
	offsetY *= sensitivity;
	const float offSetZ = z * sensitivity;
	
	//add rotations
	m_rotations.y += offsetX;
	m_rotations.x += offsetY;
	m_rotations.z += offSetZ;
	
	//limit the pitch and roll
	if (m_rotations.x > 89.0f) m_rotations.x = 89.0f;
	else if (m_rotations.x < -89.0f) m_rotations.x = -89.0f;
	if (m_rotations.z > 89.0f) m_rotations.z = 89.0f;
	else if (m_rotations.z < -89.0f) m_rotations.z = -89.0f;

}
