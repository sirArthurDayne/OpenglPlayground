#include "MeshTest.h"
#include "../imgui/imgui.h"
#include"gtc/matrix_transform.hpp"

test::MeshTest::MeshTest(GLFWwindow*& win) :
	m_win(win),
	m_cameraPos(glm::vec3(glm::vec3(0.0f, 0.0f, 15.0f))),
	m_cubeTranslation(glm::vec3(0.0f)),
	m_cubeScale(glm::vec3(1.0f)),
	m_cameraTarget(glm::vec3(0.0f)),
	m_MyCamera (Camera(m_cameraPos, m_cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3((float)WIDTH /2.0f, (float)HEIGHT /2.0f, 0.0f))),
	m_view(glm::mat4(1.0f)), m_ColorBase(glm::vec3(1.0f, 1.0f, 1.0f)),
	m_lightPos(glm::vec3(0.0f,	1.0f, -1.0f)), m_lightColor(glm::vec3(1.0f))
{
	//enable all features
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CW);
	//setup data
	glm::vec3 tri_pos[] =
	{
		{glm::vec3(-1.0f, -1.0f, 1.0f)},//0
		{glm::vec3(-1.0f, -1.0f, -1.0f)},//1
		{glm::vec3(1.0f, -1.0f,-1.0f)},//2
		{glm::vec3(1.0f, -1.0f, 1.0f)},//3

		{glm::vec3(-1.0f, 1.0f,1.0f)},//4
		{glm::vec3(-1.0f,1.0f,-1.0f)},//5
		{glm::vec3(1.0f,1.0f,-1.0f)},//6
		{glm::vec3(1.0f,1.0f,1.0f)},//7
	};
	glm::vec2 tex_pos[] =
	{
		{glm::vec2(0.0f, 0.0f)},//0
		{glm::vec2(1.0f, 0.0f)},//1
		{glm::vec2(0.0f, 1.0f)},//2
		{glm::vec2(1.0f, 1.0f)},//3
	};
	glm::vec3 tri_normals[] =
	{
		{0.0f,-1.0f,0.0f},//bottom
		{-1.0f,0.0f,0.0f},//left
		{1.0f,0.0f,0.0f},//right
		{0.0f,0.0f,-1.0f},//back
		{0.0f,0.0f,1.0f},//front
		{0.0f,1.0f,0.0f},//top
	};
	std::vector<Vertex> data =//pos, tex, norm
	{
		//bottom
		{tri_pos[2],tex_pos[0],tri_normals[0]},//0
		{tri_pos[1],tex_pos[1],tri_normals[0]},//1
		{tri_pos[3],tex_pos[2],tri_normals[0]},//2
		{tri_pos[0],tex_pos[3],tri_normals[0]},//3
		//left
		{tri_pos[5],tex_pos[0],tri_normals[1]},//4
		{tri_pos[4],tex_pos[1],tri_normals[1]},//5
		{tri_pos[1],tex_pos[2],tri_normals[1]},//6
		{tri_pos[0],tex_pos[3],tri_normals[1]},//7
		//right
		{tri_pos[7],tex_pos[0],tri_normals[2]},//8
		{tri_pos[6],tex_pos[1],tri_normals[2]},//9
		{tri_pos[3],tex_pos[2],tri_normals[2]},//10
		{tri_pos[2],tex_pos[3],tri_normals[2]},//11
		//back
		{tri_pos[6],tex_pos[0],tri_normals[3]},//12
		{tri_pos[5],tex_pos[1],tri_normals[3]},//13
		{tri_pos[2],tex_pos[2],tri_normals[3]},//14
		{tri_pos[1],tex_pos[3],tri_normals[3]},//15
		//front
		{tri_pos[4],tex_pos[0],tri_normals[4]},//16
		{tri_pos[7],tex_pos[1],tri_normals[4]},//17
		{tri_pos[0],tex_pos[2],tri_normals[4]},//18
		{tri_pos[3],tex_pos[3],tri_normals[4]},//19
		//top
		{tri_pos[5],tex_pos[0],tri_normals[5]},//20
		{tri_pos[6],tex_pos[1],tri_normals[5]},//21
		{tri_pos[4],tex_pos[2],tri_normals[5]},//22
		{tri_pos[7],tex_pos[3],tri_normals[5]},//23
	};

	std::vector<unsigned int> indices =
	{
		//bottom
		2,0,1,
		2,1,3,
		//left
		6,4,5,
		6,5,7,
		//right
		10,8,9,
		10,9,11,
		//back
		14,12,13,
		14,13,15,
		//front
		18,16,17,
		18,17,19,
		//top
		22,20,21,
		22,21,23
	};

	
	//setup light object
	m_lightCube = new Mesh(data, indices);
	//setup model
	m_MyModel = new Model("models/Almeja/almeja.obj", true);
	
	//setup shaders and textures
	m_fongLightShader = new Shader("shaders/FongLighting.shader");
	m_lightSourceShader = new Shader("shaders/LightSource.shader");
	m_gouraudLightShader = new Shader("shaders/GouraudLighting.shader");
	m_normalMapShader = new Shader("shaders/NormalMapLighting.shader");
	
	m_fongLightShader->Unbind();
	m_lightSourceShader->Unbind();
	m_gouraudLightShader->Unbind();
	m_normalMapShader->Unbind();
}

test::MeshTest::~MeshTest()
{
	delete m_fongLightShader, m_lightSourceShader, m_gouraudLightShader, m_normalMapShader;
	delete m_lightCube, m_MyModel;
	//set default enviroment
	glDisable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	system("cls");
}

void test::MeshTest::OnRenderer()
{
	Renderer renderer;
	renderer.Clear(0.0f, 0.0, 0.0f);
	//bind stuff
	
	Lighting lightState;
	if (m_shaderActive == 0) lightState = Lighting::PHONG;
	else if (m_shaderActive == 1) lightState = Lighting::GOURAUD;
	else lightState = Lighting::NORMAL_MAP;
	
	BindSelectedShader(lightState);

	m_MyModel->DrawModel(renderer, m_fongLightShader);
	
	//light sourceCube
	{
		m_lightSourceShader->Bind();
		m_lightSourceShader->SetUniform3f("u_lightColor", m_lightColor.x, m_lightColor.y, m_lightColor.z);
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), m_lightPos);
		glm::mat4 rotate = glm::mat4(1.0f);// glm::rotate(glm::mat4(1.0f), glm::radians(10.0f) * float(glfwGetTime()), glm::vec3(.20f, 0.30f, .40f));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
		model *= trans * rotate * scale;
		glm::mat4 proy = glm::perspective(glm::radians(m_FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 mvp = proy * m_view * model;
		m_lightSourceShader->SetUniformMat4f("u_mvp", mvp);
		m_lightCube->Draw(renderer, m_lightSourceShader);
	}
}

void test::MeshTest::OnUserUpdate(float deltaTime)
{
	Camera_Movement move = Camera_Movement::IDLE;
	m_MyCamera.setPosition(m_cameraPos);
	m_MyCamera.UpdateCamera(move, 0.0f, deltaTime);
	m_view = m_MyCamera.GetViewMatrix();
	m_lightPos.x = sin(float(glfwGetTime())) * 2.0f + 1.0f;
	//m_lightPos.y = sin(float(glfwGetTime() / 2.0f)) * 2.0f + 1.0f;
	m_lightPos.z = cos(float(glfwGetTime())) * 2.0f + 4.0f;
}

void test::MeshTest::OnGuiRenderer()
{
	ImGui::Begin("Model Inspector");
	ImGui::SliderFloat("FOV", &m_FOV, 30.0f, 90.0f);
	ImGui::SliderFloat3("cameraPosition", &m_cameraPos.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("scale", &m_cubeScale.x, .50f, 3.0f);
	ImGui::SliderFloat3("translation", &m_cubeTranslation.x, -20.0f, 20.0f);
	ImGui::ColorEdit3("Base color", &m_ColorBase.x);
	ImGui::ColorEdit3("Ambient", &m_MyMaterials.ambient.x);
	ImGui::ColorEdit3("Diffuse", &m_MyMaterials.diffuse.x);
	ImGui::ColorEdit3("Specular", &m_MyMaterials.specular.x);
	ImGui::SliderFloat("shininess", &m_MyMaterials.shininess, 0.0f, 1.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	
	ImGui::Begin("Shading Inspector");
	ImGui::RadioButton("Phong", &m_shaderActive, 0);
	ImGui::SameLine();
	ImGui::RadioButton("Gouraud", &m_shaderActive, 1);
	ImGui::SameLine();
	ImGui::RadioButton("NormalMap", &m_shaderActive, 2);
	ImGui::SliderFloat3("position", &m_lightPos.x, -10.0f, 10.0f);
	ImGui::ColorEdit3("color", &m_lightColor.x);
	ImGui::End();
}

void test::MeshTest::BindSelectedShader(Lighting& option)
{
	if (option == Lighting::PHONG)
	{
		m_fongLightShader->Bind();
		UpdateScene(m_fongLightShader);
			
	}
	else if (option == Lighting::GOURAUD)
	{
		m_gouraudLightShader->Bind();
		UpdateScene(m_gouraudLightShader);
	}
	else if (option == Lighting::NORMAL_MAP)
	{
		m_normalMapShader->Bind();
		UpdateScene(m_normalMapShader);
	}
}

void test::MeshTest::UpdateScene(Shader* shader)
{
	//TODO: Send data to Mesh class and return ModelMatrix
	glm::mat4 trans  = glm::translate(glm::mat4(1.0f), m_cubeTranslation);
	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f) * float(glfwGetTime()), glm::vec3(.20f, 0.30f, .40f));
	//glm::mat4 rotate = glm::mat4(1.0f);
	glm::mat4 scale  = glm::scale(glm::mat4(1.0f), m_cubeScale);
	glm::mat4 model  = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
	model *= trans * rotate * scale;
	
	glm::mat4 proy = glm::perspective(glm::radians(m_FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	glm::mat4 mvp = proy * m_view * model;
	//materials
	shader->SetUniform3f("u_material.ka", m_MyMaterials.ambient.x, m_MyMaterials.ambient.y, m_MyMaterials.ambient.z);
	shader->SetUniform3f("u_material.kd", m_MyMaterials.diffuse.x, m_MyMaterials.diffuse.y, m_MyMaterials.diffuse.z);
	shader->SetUniform3f("u_material.ks", m_MyMaterials.specular.x, m_MyMaterials.specular.y, m_MyMaterials.specular.z);
	shader->SetUniform1f("u_material.sh", m_MyMaterials.shininess);
	//vectors
	shader->SetUniform3f("u_colorBase", m_ColorBase.x, m_ColorBase.y, m_ColorBase.z);
	shader->SetUniform3f("u_lightPosition", m_lightPos.x, m_lightPos.y, m_lightPos.z);
	shader->SetUniform3f("u_lightColor", m_lightColor.x, m_lightColor.y, m_lightColor.z);
	shader->SetUniform3f("u_cameraPosition", m_cameraPos.x, m_cameraPos.y, m_cameraPos.z);
	//matrices
	shader->SetUniformMat4f("u_mvp", mvp);
	shader->SetUniformMat4f("u_model", model);
	shader->SetUniformMat4f("u_view", m_view);
	
}
