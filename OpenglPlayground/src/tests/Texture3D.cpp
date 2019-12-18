﻿#include "Texture3D.h"
#include"gtc/matrix_transform.hpp"
#include"../Renderer.h"
#include "../VertexLayout.h"
#include "../imgui/imgui.h"
#include <GLFW/glfw3.h>
#include"../Vertex.h"
float fov = 45.0f;

test::Texture3D::Texture3D(GLFWwindow*& win):
	m_win(win),
	m_FOV(45.0f),
	m_cameraPos(glm::vec3(0.0f, 0.0f, 15.0f)),
	m_proy(glm::perspective(glm::radians(m_FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f)),
	m_view(glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f),//def cameraPos
		glm::vec3(0.0f, 0.0f, 0.0f),//def target
		glm::vec3(0.0f, 1.0f, 0.0f))),//def up vec
	m_rotate(glm::rotate(glm::mat4(1.0), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f))),
	m_scale(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f))),
	m_translationVec(glm::vec3(0.0f)), m_scaleVec(1.0f, 1.0f, 1.0f),
	m_deltaTime(0.0f), m_cameraSpeed(5.0f),
	m_LastPos(glm::vec3(float(WIDTH) / 2.0f, float(HEIGHT) / 2.0f,0.0f)),
	m_EulerRotation(glm::vec3(0.0f, -90.0f, 90.0f))
{
	glm::vec3 tri_pos[] =
	{
		{glm::vec3(-1.0f, -1.0f, -1.0f)	/2.0f},//0
		{glm::vec3(-1.0f, 1.0f, -1.0f)	/2.0f},//1
		{glm::vec3(1.0f, 1.0f,-1.0f)		/2.0f},//2
		{glm::vec3(1.0f, -1.0f, -1.0f)	/2.0f},//3
									  
		{glm::vec3(-1.0f, -1.0f,1.0f)	/2.0f},//4
		{glm::vec3(-1.0f,1.0f,1.0f)	/2.0f	},//5
		{glm::vec3(1.0f,1.0f,1.0f)	/2.0f	},//6
		{glm::vec3(1.0f,-1.0f,1.0f)	/2.0f	},//7
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
  glm::vec3(0.0f, 0.0f, 0.0f),
glm::vec3(0.0f, 1.0f, 0.0f),
glm::vec3(0.0f, 2.0f, 0.0f),
glm::vec3(1.0f, 3.0f, 0.0f),
glm::vec3(2.0f, 3.0f, 0.0f),
glm::vec3(3.0f, 2.0f, 0.0f),
glm::vec3(0.0f, -1.0f, 0.0f),
glm::vec3(0.0f, -2.0f, 0.0f),
glm::vec3(1.0f, -3.0f, 0.0f),
glm::vec3(2.0f, -3.0f, 0.0f),
glm::vec3(3.0f, -2.0f, 0.0f),
glm::vec3(5.0f, 0.0f, 0.0f),
glm::vec3(6.0f, 0.0f, 0.0f),
glm::vec3(7.0f, 0.0f, 0.0f),
glm::vec3(6.0f, 1.0f, 0.0f),
glm::vec3(6.0f, -1.0f, 0.0f),
glm::vec3(9.0f, 0.0f, 0.0f),
glm::vec3(10.0f, 0.0f, 0.0f),
glm::vec3(11.0f, 0.0f, 0.0f),
glm::vec3(10.0f, 1.0f, 0.0f),
glm::vec3(10.0f, -1.0f, 0.0f)
	};
	
	//enable all features
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_CW);
	//GLCALL(glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED));
	glfwSetCursorPos(m_win, m_LastPos.x, m_LastPos.y);
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
	glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	fov = 45.0f;
}


void MouseCallBack(GLFWwindow* win, double& x, double& y)
{
	GLCALL(glfwGetCursorPos(win, &x, &y));
}

void scrollCallback(GLFWwindow* win, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f) fov -= yoffset;
	if (fov > 45.0f) fov = 45.0f;
	else if (fov < 1.0f) fov = 1.0f;
}
void test::Texture3D::OnRenderer()
{
	Renderer renderer;
	renderer.Clear(0.10f, 0.10f, 0.80f);
	glClear( GL_DEPTH_BUFFER_BIT);
	
	double mouseX = 0.0f;
	double mouseY = 0.0f;
	MouseCallBack(m_win, mouseX, mouseY);
	glfwSetScrollCallback(m_win, scrollCallback);
	
	//shader binding and uniform sending data
	m_texture->Bind();
	m_texture2->Bind(1);

	m_shader->Bind();

	m_shader->SetUniform2f("u_resolution", (float)WIDTH, (float)HEIGHT);
	m_shader->SetUniform1f("u_time", float(glfwGetTime()));

	m_FOV = fov;
	//calculate camera view
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(m_cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

	//camera input
	float offsetZ = 0.0f;
	Movement(cameraFront, cameraUp, offsetZ);
	MoveRotation(cameraFront,cameraUp, mouseX, mouseY, offsetZ);
	//set camera view
	m_view = glm::lookAt(m_cameraPos, m_cameraPos + cameraFront, cameraUp);
	
	for (int i = 0; i < m_cubePos.size(); i++)
	{
		float angle = 20.0f * i + 5.0f;
		m_proy = glm::perspective(glm::radians(m_FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		m_scale = glm::scale(glm::mat4(1.0f), m_scaleVec);
		m_rotate = glm::rotate(glm::mat4(1.0f), glm::radians(angle) * float(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_cubePos[i]);
		glm::mat4 trans = glm::translate(glm::mat4(1.0), m_translationVec);
		model *=  trans * m_rotate * m_scale;
		glm::mat4 mvp = m_proy * m_view * model;
		m_shader->SetUniformMat4f("u_mvp", mvp);
		renderer.Draw(m_VAO, m_IBO);
	}

}

void test::Texture3D::OnUserUpdate(float deltaTime)
{
	m_deltaTime = deltaTime;
}

void test::Texture3D::OnGuiRenderer()
{
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	ImGui::Begin("Move Cube!");
	ImGui::SliderFloat("FOV", &m_FOV, 1.0f, 45.0f);
	ImGui::SliderFloat3("world camera", &m_cameraPos.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("translation", &m_translationVec.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("scale", &m_scaleVec.x, 0.50f, 3.0);
	ImGui::SliderFloat3("pitch-yaw-roll", &m_EulerRotation.x, -90.0f, 90.0f);
	ImGui::SliderFloat("cameraSpeed", &m_cameraSpeed, 0.0f, 10.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void test::Texture3D::Movement(const glm::vec3& camFront, const glm::vec3& camUp, float& zOffset)
{
	float speed = m_cameraSpeed * m_deltaTime;
	if (glfwGetKey(m_win, GLFW_KEY_W) == GLFW_PRESS) m_cameraPos += speed *camFront;
	else if (glfwGetKey(m_win, GLFW_KEY_S) == GLFW_PRESS) m_cameraPos -= speed * camFront;
	if (glfwGetKey(m_win, GLFW_KEY_A) == GLFW_PRESS) m_cameraPos -= glm::normalize(glm::cross(camFront, camUp)) * speed;
	else if (glfwGetKey(m_win, GLFW_KEY_D) == GLFW_PRESS) m_cameraPos += glm::normalize(glm::cross(camFront, camUp)) * speed;
	if (glfwGetKey(m_win, GLFW_KEY_LEFT) == GLFW_PRESS) zOffset -= 1.0f;
	else if (glfwGetKey(m_win, GLFW_KEY_RIGHT) == GLFW_PRESS) zOffset += 1.0f;
	
}

void test::Texture3D::MoveRotation(glm::vec3& camFront, glm::vec3& cameraUp, const double& x, const double& y, float& z)
{
	//get distance from last frames & update
	float offsetX = float(x) - m_LastPos.x;
	float offsetY = m_LastPos.y - float(y);
	m_LastPos.x = float(x);
	m_LastPos.y = float(y);

	float sensitivity = 0.15f;
	offsetX *= sensitivity;
	offsetY *= sensitivity;
	float offSetZ = z * sensitivity;
	/*update rotations.
	 *Euler Rotations are perpendicular to the axis
	PITCH: x axis rotation
	YAW: y axis rotation
	ROLL: z axis rotation*/
	m_EulerRotation.y += offsetX;
	m_EulerRotation.x += offsetY;
	m_EulerRotation.z += offSetZ;
	
	//limit the pitch and roll
	std::clamp(m_EulerRotation.x, -89.0f, 89.0f);		
	std::clamp(m_EulerRotation.z, -89.0f, 89.0f);		
	//calculate rotations
	cameraUp.x = cos(glm::radians(m_EulerRotation.z));
	cameraUp.y = sin(glm::radians(m_EulerRotation.z));
	//send to cameraUp
	cameraUp = glm::normalize(cameraUp);
	
	glm::vec3 front;
	front.x = cos(glm::radians(m_EulerRotation.x)) * cos(glm::radians(m_EulerRotation.y));
	front.y = sin(glm::radians(m_EulerRotation.x));
	front.z = cos(glm::radians(m_EulerRotation.x)) * sin(glm::radians(m_EulerRotation.y));
	//send to cameraFront
	camFront = glm::normalize(front);
}

