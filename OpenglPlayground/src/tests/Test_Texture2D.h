#pragma once
#include "Test.h"
#include "glm.hpp"
#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../Shader.h"
#include "../Texture.h"
namespace test 
{
	class Texture2D : public Test
	{
	public:
		Texture2D(GLFWwindow*& win);
		~Texture2D();
		void OnRenderer() override;
		void OnGuiRenderer() override;
	private:
		GLFWwindow*& m_win;
		VertexArray* m_VAO;
		VertexLayout* m_VL;
		VertexBuffer* m_VBO;
		IndexBuffer* m_IBO;
		Shader* m_shader;
		Texture* m_texture,* m_texture2;
		glm::mat4 m_proy, m_view, m_rotate, m_scale;
		glm::vec3 m_translationA, m_translationB;
		glm::vec3 m_scaleVec;
	};
}