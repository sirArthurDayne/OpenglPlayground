#include "MeshTest.h"
#include "../imgui/imgui.h"
#include"gtc/matrix_transform.hpp"

test::MeshTest::MeshTest(GLFWwindow*& win) :
	m_win(win),
	m_cameraPos(glm::vec3(glm::vec3(0.0f, 0.0f, 5.0f))),
	m_cubeTranslation(glm::vec3(0.0f)),
	m_cubeScale(glm::vec3(1.0f)),
	m_cameraTarget(glm::vec3(0.0f)),
	m_MyCamera (Camera(m_cameraPos, m_cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f))),
	m_view(glm::mat4(1.0f)), m_ColorBase(glm::vec3(0.0f, 0.60f, 0.20f))
{
	//enable all features
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CW);
	//setup data
	glm::vec3 tri_pos[] =
	{
		{glm::vec3(-1.0f, -1.0f, -1.0f) / 2.0f},//0
		{glm::vec3(-1.0f, 1.0f, -1.0f) / 2.0f},//1
		{glm::vec3(1.0f, 1.0f,-1.0f) / 2.0f},//2
		{glm::vec3(1.0f, -1.0f, -1.0f) / 2.0f},//3

		{glm::vec3(-1.0f, -1.0f,1.0f) / 2.0f},//4
		{glm::vec3(-1.0f,1.0f,1.0f) / 2.0f	},//5
		{glm::vec3(1.0f,1.0f,1.0f) / 2.0f	},//6
		{glm::vec3(1.0f,-1.0f,1.0f) / 2.0f	},//7
	};
	glm::vec2 tex_pos[] =
	{
		{glm::vec2(0.0f, 0.0f)},//0
		{glm::vec2(1.0f, 0.0f)},//1
		{glm::vec2(0.0f, 1.0f)},//2
		{glm::vec2(1.0f, 1.0f)},//3
	};
	std::vector<Vertex> data =//pos, tex, norm
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

	std::vector<unsigned int> indices =
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

	//setup mesh
	m_geo = new Mesh(data, indices);
	
	//setup shaders and textures
	m_shader = new Shader("shaders/LightTest.shader");
	m_shader->SetUniform1i("u_texture0", 0);
	m_shader->SetUniform1i("u_texture1", 1);
	m_shader->Unbind();
	//TEXTURES
	m_texture = new Texture("rusty.png");
	m_texture2 = new Texture("character.png");
}

test::MeshTest::~MeshTest()
{
	delete m_shader;
	delete m_texture;
	delete m_texture2;
	delete m_geo;
	//set default enviroment
	glDisable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
}

void test::MeshTest::OnRenderer()
{
	Renderer renderer;
	renderer.Clear(0.0f, 0.0, 0.0f);
	glm::vec3 origin = glm::vec3(0.0f);
	//bind stuff
	m_texture->Bind();
	m_texture2->Bind(1);
	m_shader->Bind();
	m_shader->SetUniform1f("u_time", float(glfwGetTime()));
	m_shader->SetUniform3f("u_colorBase", m_ColorBase.x, m_ColorBase.y, m_ColorBase.z);
	//update stuff
	//TODO: Send data to Mesh class and return ModelMatrix
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), m_cubeTranslation);
	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f)  * float(glfwGetTime()), glm::vec3(.20f, 0.30f, .40f));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_cubeScale);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), origin);
	model *= trans * rotate * scale;
	m_view = m_MyCamera.GetViewMatrix();
	glm::mat4 proy = glm::perspective(glm::radians(m_FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	glm::mat4 mvp = proy * m_view * model;
	
	//draw stuff
	m_shader->SetUniformMat4f("u_mvp", mvp);
	m_geo->Draw(renderer);
}

void test::MeshTest::OnGuiRenderer()
{
	ImGui::Begin("Transformations");
	ImGui::SliderFloat("FOV", &m_FOV, 30.0f, 90.0f);
	ImGui::SliderFloat3("scale", &m_cubeScale.x, 1.0f, 3.0f);
	ImGui::SliderFloat3("translation", &m_cubeTranslation.x, -10.0f, 10.0f);
	ImGui::ColorEdit3("base color", &m_ColorBase.x);
	ImGui::End();
}
