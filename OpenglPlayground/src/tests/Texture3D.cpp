#include "Texture3D.h"
#include"gtc/matrix_transform.hpp"
#include"../Renderer.h"
#include "../VertexLayout.h"
#include "../imgui/imgui.h"
#include <GLFW/glfw3.h>
#include"../Vertex.h"


test::Texture3D::Texture3D():
	m_FOV(45.0f), m_rotation(-55.0f),
	m_camera(glm::vec3(0.0f, 0.0f, -6.0f)),
	m_proy(glm::perspective(glm::radians(m_FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f)),
	m_view(glm::translate(glm::mat4(1.0f), m_camera)),
	m_rotate(glm::rotate(glm::mat4(1.0), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f))),
	m_scale(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f))),
	m_translationA(glm::vec3(0.0f)), m_scaleVec(1.0f, 1.0f, 1.0f)
{
	glm::vec3 tri_pos[] =
	{
		{glm::vec3(-1.0f, -1.0f, -1.0f)	},//0
		{glm::vec3(-1.0f, 1.0f, -1.0f)	},//1
		{glm::vec3(1.0f, 1.0f,-1.0f)		},//2
		{glm::vec3(1.0f, -1.0f, -1.0f)	},//3
									  
		{glm::vec3(-1.0f, -1.0f,1.0f)	},//4
		{glm::vec3(-1.0f,1.0f,1.0f)		},//5
		{glm::vec3(1.0f,1.0f,1.0f)		},//6
		{glm::vec3(1.0f,-1.0f,1.0f)		},//7
	};
	glm::vec2 tex_pos[] =
	{
		{glm::vec2(0.0f, 0.0f)},//0
		{glm::vec2(1.0f, 0.0f)},//1
		{glm::vec2(0.0f, 1.0f)},//2
		{glm::vec2(1.0f, 1.0f)},//3
	};

	Vertex cube[] = 
	{
		//bottom
		{tri_pos[0],tex_pos[2]},//0
		{tri_pos[1],tex_pos[0]},//1
		{tri_pos[2],tex_pos[1]},//2
		{tri_pos[3],tex_pos[3]},//3
		//left
		{tri_pos[0],tex_pos[2]},//4
		{tri_pos[4],tex_pos[0]},//5
		{tri_pos[5],tex_pos[1]},//6
		{tri_pos[1],tex_pos[3]},//7
		//right
		{tri_pos[2],tex_pos[2]},//8
		{tri_pos[6],tex_pos[0]},//9
		{tri_pos[7],tex_pos[1]},//10
		{tri_pos[3],tex_pos[3]},//11
		//back
		{tri_pos[1],tex_pos[2]},//12
		{tri_pos[5],tex_pos[0]},//13
		{tri_pos[6],tex_pos[1]},//14
		{tri_pos[2],tex_pos[3]},//15
		//front
		{tri_pos[0],tex_pos[2]},//16
		{tri_pos[4],tex_pos[0]},//17
		{tri_pos[7],tex_pos[1]},//18
		{tri_pos[3],tex_pos[3]},//19
		//top
		{tri_pos[4],tex_pos[2]},//20
		{tri_pos[5],tex_pos[0]},//21
		{tri_pos[6],tex_pos[1]},//22
		{tri_pos[7],tex_pos[3]},//23
	};
	unsigned int indices[] =
	{
		//bottom
		0,1,2,
		3,0,2,
		//left
		4,5,6,
		7,4,6,
		//right
		8,9,10,
		11,8,10,
		//back
		12,13,14,
		15,12,14,
		//front
		16,17,18,
		19,16,18,
		//top
		20,21,22,
		23,20,22
	};

	m_cubePos = {
  //m_translationA,
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -32.5f),
  glm::vec3(3.8f, -2.0f, -12.3f),
  glm::vec3(3.4f, -0.4f, -32.5f),
  glm::vec3(1.7f,  3.0f, -27.5f),
	};
	
	//enable all features
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CW);
	//glEnable(GL_CULL_FACE);
	
	m_VAO = new VertexArray();
	m_VL = new VertexLayout();
	m_VL->Push<float>(3);//pos
	m_VL->Push<float>(2);//texture

	m_VBO = new VertexBuffer(cube,  6 * 4 * sizeof(Vertex));
	m_VAO->AddBuffer(*m_VBO, *m_VL);

	m_IBO = new IndexBuffer(indices, sizeof(indices)/ sizeof(indices[0]));

	m_shader = new Shader("shaders/Base.shader");
	m_shader->SetUniform1i("u_texture0", 0);
	m_shader->SetUniform1i("u_texture1", 1);
	//TEXTURES
	m_texture = new Texture("fabric.png");
	m_texture2 = new Texture("piratelogo.png");

	//UNBOUND EVERYTHING
	m_VAO->Unbind();
	m_VBO->Unbind();
	m_IBO->Unbind();
	m_shader->Unbind();
	
}

test::Texture3D::~Texture3D()
{
	delete m_VBO;
	delete m_IBO;
	delete m_VL;
	delete m_VAO;
	delete m_shader;
	delete m_texture;
	delete m_texture2;
	//disable all features
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

}

void test::Texture3D::OnRenderer()
{
	Renderer renderer;
	renderer.Clear(0.80f, 0.30f, 0.50f);
	glClear( GL_DEPTH_BUFFER_BIT);
	//shader binding and uniform sending data
	m_texture->Bind();
	m_texture2->Bind(1);

	m_shader->Bind();

	m_shader->SetUniform2f("u_resolution", (float)WIDTH, (float)HEIGHT);
	m_shader->SetUniform1f("u_time", float(glfwGetTime()));

	for (int i = 0; i < m_cubePos.size(); i++)
	{
		float angle = 20.0f * i + 5.0f;
		if (i % 3 == 0)  // every 3rd iteration (including the first) we set the angle using GLFW's time function.
			angle = float(glfwGetTime()) * 25.0f;
		m_proy = glm::perspective(glm::radians(m_FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		m_scale = glm::scale(glm::mat4(1.0f), m_scaleVec);
		m_view = glm::translate(glm::mat4(1.0f), m_camera);
		m_rotate = glm::rotate(glm::mat4(1.0f), glm::radians(angle) * float(glfwGetTime()), glm::vec3(1.0f, 0.30f, 0.50f));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_cubePos[i]);
		glm::mat4 trans = glm::translate(glm::mat4(1.0), m_translationA);
		model *=  trans * m_rotate * m_scale;
		glm::mat4 mvp = m_proy * m_view * model;
		m_shader->SetUniformMat4f("u_mvp", mvp);
		renderer.Draw(m_VAO, m_IBO);
	}

}

void test::Texture3D::OnGuiRenderer()
{
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	ImGui::Begin("Move Cube!");
	ImGui::SliderFloat("FOV", &m_FOV, 30.0f, 120.0f);
	ImGui::SliderFloat3("camera X", &m_camera.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("coordenadas", &m_translationA.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("escala", &m_scaleVec.x, 0.50f, 3.0);
	ImGui::SliderFloat("rotacion", &m_rotation, -180.0f, 180.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}
