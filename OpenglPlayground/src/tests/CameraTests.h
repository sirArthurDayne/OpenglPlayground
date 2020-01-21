#pragma once
#include "Test.h"
#include "../Model.h"
#include"../MayaCamera.h"
#include"../Shader.h"

namespace test
{
	
	class CameraTest : public Test
	{
	public:
		CameraTest(GLFWwindow* &win);
		~CameraTest();
		void OnRenderer() override;
		void OnUserUpdate(float deltaTime) override;
		void OnGuiRenderer() override;
		void UpdateScene();
	private:
		GLFWwindow* &m_win;
		Model* TestModel;
		Mesh* lightCube;
		Shader* m_fong_ligthing_shader, *m_lightSource_shader;
		MayaCamera* FreeCamera;

		glm::vec3 lightCubePos;
		glm::vec3 att;
		glm::mat4 m_viewMatrix;
		
	};

}