#pragma once
#include"Test.h"
#include"glm.hpp"
#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../Shader.h"
#include "../Texture.h"
#include<array>
namespace test
{
	class Texture3D : public Test
	{
	public:
		Texture3D();
		~Texture3D();
		void OnRenderer() override;
		void OnGuiRenderer() override;
	private:
		VertexArray* m_VAO;
		VertexLayout* m_VL;
		VertexBuffer* m_VBO;
		IndexBuffer* m_IBO;
		Shader* m_shader;
		Texture* m_texture, *m_texture2;
		glm::mat4 m_proy, m_view, m_rotate, m_scale;
		float m_FOV, m_rotation;
		glm::vec3 m_translationA, m_camera;
		glm::vec3 m_scaleVec;
		std::array<glm::vec3, 5> m_cubePos;
	};
}
