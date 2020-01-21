#include "CameraTests.h"
#include "../imgui/imgui.h"

test::CameraTest::CameraTest(GLFWwindow*& win)
	: m_win(win)
{

	//enable all features
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CW);

	glfwSetCursorPos(m_win,float(WIDTH)/2.0f, float(HEIGHT)/2.0f);
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
	lightCube = new Mesh(data, indices);
	TestModel = new Model("models/Nanosuit/nanosuit.obj", true);
	
	m_fong_ligthing_shader = new Shader("shaders/FongLighting.shader");
	m_fong_ligthing_shader->Unbind();
	m_lightSource_shader = new Shader("shaders/LightSource.shader");
	m_lightSource_shader->Unbind();

	//FreeCamera = new MayaCamera(glm::vec3(0.0f,0.0f,15.0f), 45.0f);
	FirstPersonCamera = new FPSCamera(glm::vec3(0.0f, 0.0f, 15.0f), 45.0f);
	
	lightCubePos = glm::vec3(0.0f, 1.0f, 1.0f);

	att = glm::vec3(1.0f, 0.045f, 0.007f);	

}

test::CameraTest::~CameraTest()
{
	delete lightCube, TestModel;
	delete m_fong_ligthing_shader, m_lightSource_shader;
//	delete FreeCamera;
	delete FirstPersonCamera;
	
	//set default enviroment
	glDisable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	GLCALL(glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_NORMAL));
	system("cls");

}

void test::CameraTest::OnRenderer()
{
	Renderer renderer;
	renderer.Clear(0.0f, 0.0f, 0.0f);

	m_fong_ligthing_shader->Bind();
	
	
	UpdateScene();
	TestModel->DrawModel(renderer, m_fong_ligthing_shader);

	//light sourceCube
	{
		m_lightSource_shader->Bind();
		m_lightSource_shader->SetUniform3f("u_lightColor", 1.0f, 1.0f, 1.0f);
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), lightCubePos);
		glm::mat4 rotate = glm::mat4(1.0f);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.45f));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
		model *= trans * rotate * scale;
		glm::mat4 proy = glm::perspective(glm::radians(FirstPersonCamera->GetCameraFOV()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 mvp = proy * m_viewMatrix * model;
		
		m_lightSource_shader->SetUniformMat4f("u_mvp", mvp);
		lightCube->Draw(renderer, m_lightSource_shader);
	}

	//plane 
	{
		m_lightSource_shader->Bind();
		m_lightSource_shader->SetUniform3f("u_lightColor", 0.10f, 0.40f, 0.30f);
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,-2.0f,0.0f));
		glm::mat4 rotate = glm::mat4(1.0f);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(15.0f,0.10f,15.0f));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
		model *= trans * rotate * scale;
		glm::mat4 proy = glm::perspective(glm::radians(FirstPersonCamera->GetCameraFOV()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 mvp = proy * m_viewMatrix * model;
		
		m_lightSource_shader->SetUniformMat4f("u_mvp", mvp);
		lightCube->Draw(renderer, m_lightSource_shader);

	}

}


void test::CameraTest::OnUserUpdate(float deltaTime)
{
	FirstPersonCamera->UpdateCamera(m_win, deltaTime);
	m_viewMatrix = FirstPersonCamera->GetViewMatrix();

	lightCubePos.x = sin(float(glfwGetTime())) * 2.0f + 1.0f;
	lightCubePos.z = cos(float(glfwGetTime())) * 2.0f + 4.0f;

}

void test::CameraTest::OnGuiRenderer()
{
	glm::vec3 camera = FirstPersonCamera->GetEyePosition();
	glm::vec2 rotation = FirstPersonCamera->GetPY();
	float zoom = FirstPersonCamera->GetCameraFOV();
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("position", &camera.x, -20.0f,20.0f);
	ImGui::SliderFloat2("pitch/yaw", &rotation.x, -20.0f,20.0f);
	ImGui::SliderFloat("FOV", &zoom, 1.0f, 90.0f);
	ImGui::End();
}


void test::CameraTest::UpdateScene()
{
//TODO: Send data to Mesh class and return ModelMatrix
	glm::mat4 trans  = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
	//glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f) * float(glfwGetTime()), glm::vec3(.20f, 0.30f, .40f));
	glm::mat4 rotate = glm::mat4(1.0f);
	glm::mat4 scale  = glm::scale(glm::mat4(1.0f), glm::vec3(0.50f));
	glm::mat4 model  = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
	model *= trans * rotate * scale;
	
	glm::mat4 proy = glm::perspective(glm::radians(FirstPersonCamera->GetCameraFOV()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	glm::mat4 mvp = proy * m_viewMatrix * model;

	//vectors
	m_fong_ligthing_shader->SetUniform3f("u_colorBase", 1.0f, 1.0f, 1.0f);
	m_fong_ligthing_shader->SetUniform3f("u_lightPosition", lightCubePos.x, lightCubePos.y, lightCubePos.z);
	m_fong_ligthing_shader->SetUniform3f("u_lightColor", 1.0f, 1.0f, 1.0f);
	m_fong_ligthing_shader->SetUniform3f("u_cameraPosition", FirstPersonCamera->GetEyePosition().x, FirstPersonCamera->GetEyePosition().y, FirstPersonCamera->GetEyePosition().z);
	m_fong_ligthing_shader->SetUniform3f("u_attConst", att.x,att.y, att.z);
	//matrices
	m_fong_ligthing_shader->SetUniformMat4f("u_mvp", mvp);
	m_fong_ligthing_shader->SetUniformMat4f("u_model", model);
	m_fong_ligthing_shader->SetUniformMat4f("u_view", m_viewMatrix);
	

	
}
