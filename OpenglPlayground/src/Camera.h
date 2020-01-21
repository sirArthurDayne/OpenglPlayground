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
	Camera(glm::vec3 cameraPos, glm::vec3 target, glm::vec3 up, glm::vec3 front, glm::vec3 initPoint);
	~Camera() {}
	glm::mat4 GetViewMatrix();
	void UpdateCamera(Camera_Movement& move, float speed, float dt);
	void Movement(Camera_Movement& move, float& cameraSpeed, float deltaTime);
	void MouseMovement(const float x, const float y, const float z);
	glm::vec3 getPosition() const { return m_position; }
	glm::vec3 getRotation() const { return m_rotations; }
	void setPosition(glm::vec3 pos) { m_position = pos; }
private:
	glm::vec3 m_position, m_target, m_front, m_rotations, m_LastPointAtXY;
	glm::mat4 m_rotationMat;
	glm::vec3 m_Up;
	glm::vec3 m_Right;

};
