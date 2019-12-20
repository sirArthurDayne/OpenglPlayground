#pragma once
#include"glm.hpp"

enum Camera_Movement {
	IDLE,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	Camera(glm::vec3& cameraPos, glm::vec3 target, glm::vec3 up, glm::vec3 front);
	~Camera() {}
	glm::mat4 GetViewMatrix();
	void UpdateCamera(glm::vec3 rotations, Camera_Movement& move, float speed, float dt);
	void Movement(Camera_Movement& move, float& cameraSpeed, float deltaTime);
	glm::vec3 getPosition() const { return m_position; }
private:
	glm::vec3 m_position, m_target, m_front;
	glm::mat4 m_rotationMat;
	glm::vec3 m_Up;
	glm::vec3 m_Right;

};
