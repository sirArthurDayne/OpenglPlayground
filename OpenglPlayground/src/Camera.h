#pragma once
#include"glm.hpp"


class Camera
{
public:
	Camera(glm::vec3& cameraPos, glm::vec3 target, glm::vec3 up, glm::vec3 front);
	~Camera() {}
	glm::mat4 GetViewMatrix();
	void UpdateCamera(glm::vec3 Camerapos, glm::vec3 target, glm::vec3 front, glm::vec3 rotations);

private:
	glm::mat4 m_rotationMat;
	glm::vec3 m_position, m_target, m_front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;

};
