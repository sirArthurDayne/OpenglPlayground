﻿#include "Texture3D.h"
#include"gtc/matrix_transform.hpp"
#include"../Renderer.h"
#include "../VertexLayout.h"
#include "../imgui/imgui.h"
#include <GLFW/glfw3.h>
#include"../Vertex.h"


test::Texture3D::Texture3D():
	m_FOV(45.0f), m_rotation(-55.0f),
	m_camera(glm::vec3(0.0f, 0.0f, -3.0f)),
	m_proy(glm::perspective(glm::radians(m_FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f)),
	m_view(glm::translate(glm::mat4(1.0f), m_camera)),
	m_rotate(glm::rotate(glm::mat4(1.0), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f))),
	m_scale(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f))),
	m_translationA(glm::vec3(0.0f)), m_scaleVec(1.0f, 1.0f, 1.0f)
{
	Vertex tri_pos[] =
	{
		{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 1.0f)},//0
		{glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 0.0f)},//1
		{glm::vec3(1.0f, 1.0f,-1.0f), glm::vec2(1.0f, 0.0f)},//2
		{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 1.0f)},//3
		
		{glm::vec3(-1.0f, -1.0f,1.0f), glm::vec2(0.0f, 0.0f)},//4
		{glm::vec3(-1.0f,1.0f,1.0f),glm::vec2(1.0f, 0.0f)},//5
		{glm::vec3(1.0f,1.0f,1.0f),glm::vec2(1.0f, 1.0f)},//6
		{glm::vec3(1.0f,-1.0f,1.0f),glm::vec2(1.0f, .0f)},//7
	};
	unsigned int tri_indices[] =
	{
		//bottom
		0,1,2,
		3,2,0,
		//left
		0,4,5,
		0,5,1,
		//right
		3,7,6,
		2,6,3,
		//back
		1,5,6,
		6,1,2,
		//front
		0,4,7,
		7,0,3,
		//top
		4,5,6,
		7,4,6
	};
	glEnable(GL_DEPTH_TEST);
	m_VAO = new VertexArray();
	m_VL = new VertexLayout();
	m_VL->Push<float>(3);//pos
	m_VL->Push<float>(2);//texture

	m_VBO = new VertexBuffer(tri_pos,  8 * sizeof(Vertex));
	m_VAO->AddBuffer(*m_VBO, *m_VL);

	m_IBO = new IndexBuffer(tri_indices, 36);

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

	m_proy = glm::perspective(glm::radians(m_FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	m_view = glm::translate(glm::mat4(1.0f), m_camera);


	m_scale = glm::scale(glm::mat4(1.0f), m_scaleVec);
	m_rotate = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
	model *= m_rotate * m_scale;
	glm::mat4 mvp = m_proy * m_view * model;

	m_shader->SetUniformMat4f("u_mvp", mvp);
	renderer.Draw(m_VAO, m_IBO);
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
