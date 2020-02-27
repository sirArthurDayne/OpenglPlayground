#pragma once
#include "Test.h"
#include "../Model.h"
#include"../MayaCamera.h"
#include"../FPSCamera.h"
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
		void BindSelectedShader(LIGHT_MODELS& option);
		void UpdateScene(Shader* shader);
	private:
		GLFWwindow* &m_win;

		
		Model* m_TestModel;
		Model* m_ConeModel;
		Mesh* lightCube;

		std::vector<Shader> m_lightModelShaders;
		int m_shaderActive = 0;

		MayaCamera* FreeCamera;
		FPSCamera* FirstPersonCamera;
		
		std::vector<glm::vec3> lightCubePositions, lightCubeColors;
		float cube_offset = 0.0f;
		float inneRadius = glm::radians(12.5f);
		float outerRadius = glm::radians(24.f);
		
		glm::mat4 m_viewMatrix;
		Material m_MyMaterials;
		Material m_lightMaterials;
		
		glm::vec3 att;

		
	};

}