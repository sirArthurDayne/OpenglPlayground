#include "Camera.h"
#include "gtc/matrix_transform.hpp"

Camera::Camera(glm::vec3& cameraPos, glm::vec3 target, glm::vec3 up, glm::vec3 front):
	m_position(cameraPos), m_target(target),m_Up(up), m_rotationMat(glm::mat4(1.0f)),
	m_front(front)
{
	m_Right = glm::vec3(1.0f, 0.0, 0.0f);
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::inverse(glm::translate(glm::mat4(1.0f), m_position + m_front) * m_rotationMat);
}

void Camera::UpdateCamera(glm::vec3 Camerapos,glm::vec3 target, glm::vec3 front, glm::vec3 rotations)
{
	//update camera view
	m_position = Camerapos;
	m_target = target;
	m_front = front;
	glm::vec3 cameraDirection = glm::normalize(m_position - m_target);
	m_Right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraDirection));
	m_Up = glm::cross(cameraDirection, m_Right);
	//setup rotations
	glm::mat4  rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(rotations.x), glm::vec3(1, 0, 0));
	glm::mat4  rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(rotations.y), glm::vec3(0, 1, 0));
	glm::mat4  rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotations.z), glm::vec3(0, 0, 1));
	m_rotationMat = rotationX * rotationY * rotationZ;
}
