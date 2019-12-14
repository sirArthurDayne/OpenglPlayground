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
		Texture2D();
		~Texture2D();
		void OnRenderer() override;	
		void OnGuiRenderer() override;
	private:
		VertexArray* m_VAO;
		VertexLayout* m_VL;
		VertexBuffer* m_VBO;
		IndexBuffer* m_IBO;
		Shader* m_shader;
		Texture* m_texture;
		glm::mat4 m_proy, m_view;
		glm::vec3 m_translationA, m_translationB;
	};
}