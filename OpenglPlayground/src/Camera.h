#pragma once
#include"glm.hpp"

enum class MOVEMENT
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	Camera() {  }
	~Camera() {  }
	void GetRotationMatrix(){}
	void UpdateCamera() {  }
private:
	glm::mat4 m_EulerMatrix;
	float m_FOV;
	float m_EulerAngles;
	glm::vec3 m_position;
	glm::vec3 m_upVector;
	glm::vec3 m_rightvector;
	float m_speed;
	float m_sensitivity;
};
