#pragma once
#include"Test.h"
#include"glm.hpp"
#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../Shader.h"
#include "../Texture.h"
#include<array>
#include "../Camera.h"

namespace test
{
	class Texture3D : public Test
	{
	public:
		Texture3D(GLFWwindow*& win);
		~Texture3D();
		void OnRenderer() override;
		void OnUserUpdate(float deltaTime) override;
		void OnGuiRenderer() override;
		void KeyboardMovement(const glm::vec3& camFront, const glm::vec3& camUp, float& zOffset);
		void MouseMovement(const double& x, const double& y, float& z);
		friend void MouseCallBack(GLFWwindow* win, double& x, double& y);
		friend void scrollCallback(GLFWwindow* win, double xoffset, double yoffset);
	private:
		GLFWwindow*& m_win;
		VertexArray* m_VAO;
		VertexLayout* m_VL;
		VertexBuffer* m_VBO;
		IndexBuffer* m_IBO;
		Shader* m_shader;
		Texture* m_texture, *m_texture2;
		glm::mat4 m_proy, m_view, m_rotate, m_scale;
		float m_FOV;
		glm::vec3 m_translationVec, m_cameraPos;
		glm::vec3 m_scaleVec;
		std::array<glm::vec3, 21> m_cubePos;
		float m_deltaTime;
		float m_cameraSpeed;
		glm::vec3 m_LastPos, m_EulerRotation;
		Camera MyCamera;
	};
}
