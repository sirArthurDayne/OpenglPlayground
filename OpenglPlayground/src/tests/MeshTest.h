﻿#pragma once
#include "Test.h"
#include"../Mesh.h"
#include"../camera.h"
#include "../Shader.h"
#include "../Texture.h"
namespace test
{	
	class MeshTest : public Test
	{
	public:
		MeshTest(GLFWwindow* &win);
		~MeshTest();
		void OnRenderer() override;
		void OnGuiRenderer() override;
	private:
		GLFWwindow*& m_win;
		glm::vec3 m_cameraPos, m_cameraTarget, m_cubeTranslation, m_cubeScale;
		Camera m_MyCamera;
		Mesh* m_geo;
		Shader* m_shader;
		Texture* m_texture;
		Texture *m_texture2;
		glm::mat4 m_view;
		float m_FOV = 45.0f;
		glm::vec3 m_ColorBase;
	};
}
