﻿#pragma once
#include "Test.h"
#include"../Mesh.h"
#include"../camera.h"
#include "../Shader.h"
#include "../Model.h"

namespace test
{
	class MeshTest : public Test
	{
	public:
		MeshTest(GLFWwindow* &win);
		~MeshTest();
		void OnRenderer() override;
		void OnUserUpdate(float deltaTime) override;
		void OnGuiRenderer() override;
		void BindSelectedShader(LIGHT_MODELS& option);
		void UpdateScene(Shader* shader);
	private:
		GLFWwindow*& m_win;
		glm::vec3 m_cameraPos, m_cameraTarget, m_cubeTranslation, m_cubeScale;
		glm::vec3 m_lightPos, m_lightColor, m_lightAtt;
		Camera m_MyCamera;
		Mesh* m_lightCube;
		Shader* m_fongLightShader, *m_lightSourceShader, *m_gouraudLightShader, *m_normalMapShader, *m_toonShader;
		glm::mat4 m_view;
		float m_FOV = 45.0f;
		glm::vec3 m_ColorBase;
		int m_shaderActive = 0;
		Model* m_MyModel;
		Material m_MyMaterials;
	};
}
