#include "CameraTests.h"
#include "../imgui/imgui.h"

test::CameraTest::CameraTest(GLFWwindow*& win)
	: m_win(win)
{
	//enable all features
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CW);

	glfwSetCursorPos(m_win, float(WIDTH) / 2.0f, float(HEIGHT) / 2.0f);

	GLCALL(glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED));
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
	m_TestModel = new Model("models/VideoShip.obj");
	m_ConeModel = new Model("models/cone.fbx");

	//setup shaders
	
	std::string routes[] =
	{
		"shaders/SpotLight_BlinPhong.glsl",
		"shaders/GouraudLighting.glsl",
		"shaders/NormalMapLighting.glsl",
		"shaders/Toon.glsl",
		"shaders/LightSource.glsl",
	};
	m_lightModelShaders.reserve(5);
	m_lightModelShaders.emplace_back(routes[0]);
	m_lightModelShaders.emplace_back(routes[1]);
	m_lightModelShaders.emplace_back(routes[2]);
	m_lightModelShaders.emplace_back(routes[3]);
	m_lightModelShaders.emplace_back(routes[4]);
	
	//setup cameras 
	FreeCamera = new MayaCamera(glm::vec3(0.0f, 0.0f, 15.0f), 45.0f);
	FirstPersonCamera = new FPSCamera(glm::vec3(0.0f, 0.0f, 15.0f), 45.0f);

	//setup cube pos and colors
	auto random = [](int min, int max)
	{
		static bool seeded = false;
		if (!seeded)
		{
			std::srand(time_t(NULL));
			seeded = true;
		}
		return min + std::rand() % ((max + 1)- min);
	};
	
	for (int i = 0; i < 6; i++)
	{
		float angle = i * 20.0f;
		const float offset = 5.0f;
		const float distance = 7.0f;
		glm::vec3 position(sin(angle) * distance + offset, 5.0f, cos(angle) * distance + offset);
		lightCubePositions.push_back(position);
	}
	//colors
	lightCubeColors.push_back(glm::normalize(glm::vec3(255.0f, 0.0f, 0.0f)));
	lightCubeColors.push_back(glm::normalize(glm::vec3(0.0f, 255.0f, 0.0f)));
	lightCubeColors.push_back(glm::normalize(glm::vec3(0.0f, 0.0f, 255.0f)));
	lightCubeColors.push_back(glm::normalize(glm::vec3(230.0f, 153.0f, 0.0f)));
	lightCubeColors.push_back(glm::normalize(glm::vec3(128.0f, 0.0f, 60.0f)));
	lightCubeColors.push_back(glm::normalize(glm::vec3(152.0f, 0.0f, 239.0f)));
	
	m_MyMaterials.ambient = glm::vec3(0.20f);
	m_MyMaterials.diffuse = glm::vec3(0.45f);
	m_MyMaterials.specular = glm::vec3(0.50f);
	att = glm::vec3(1.0f, 0.220f, 0.200f);	

}

test::CameraTest::~CameraTest()
{
	delete lightCube, m_TestModel;
	delete FreeCamera;
	delete FirstPersonCamera;
	
	m_lightModelShaders.clear();

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

	LIGHT_MODELS lightState;
	if (m_shaderActive == 0) lightState = LIGHT_MODELS::PHONG;
	else if (m_shaderActive == 1) lightState = LIGHT_MODELS::GOURAUD;
	else if (m_shaderActive == 2) lightState = LIGHT_MODELS::NORMAL_MAP;
	else if (m_shaderActive == 3) lightState = LIGHT_MODELS::TOON;
	
	BindSelectedShader(lightState);
	m_TestModel->DrawModel(renderer, &m_lightModelShaders[0]);


	//plane 
	{
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,-2.0f,0.0f));
		glm::mat4 rotate = glm::mat4(1.0f);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f,0.10f,20.0f));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
		model *= trans * rotate * scale;
		glm::mat4 proy = glm::perspective(glm::radians(FreeCamera->GetCameraFOV()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 mvp = proy * m_viewMatrix * model;

		//matrices
		m_lightModelShaders[m_shaderActive].SetUniformMat4f("u_mvp", mvp);
		m_lightModelShaders[m_shaderActive].SetUniformMat4f("u_model",model);
		m_lightModelShaders[m_shaderActive].SetUniformMat4f("u_view", m_viewMatrix);
		lightCube->Draw(renderer, &m_lightModelShaders[0]);
	}
	//light cones
	for (int i = 0; i < lightCubePositions.size(); i++)
	{
		m_lightModelShaders[4].Bind();
		std::string n = std::to_string(i);
		m_lightModelShaders[4].SetUniform3f("u_lightColor", lightCubeColors[i].r, lightCubeColors[i].g,lightCubeColors[i].b);
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), lightCubePositions[i] + glm::vec3(cube_offset, 0.0f, cube_offset));
		glm::mat4 rotate = glm::mat4(1.0f);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.45f));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
		model *= trans * rotate * scale;
		glm::mat4 proy = glm::perspective(glm::radians(FreeCamera->GetCameraFOV()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 mvp = proy * m_viewMatrix * model;
		
		m_lightModelShaders[4].SetUniformMat4f("u_mvp", mvp);
		m_ConeModel->DrawModel(renderer, &m_lightModelShaders[4]);
	}

}


void test::CameraTest::OnUserUpdate(float deltaTime)
{
	//FirstPersonCamera->UpdateCamera(m_win, deltaTime);
	FreeCamera->UpdateCamera(m_win, deltaTime);	
	m_viewMatrix = FreeCamera->GetViewMatrix();
}

void test::CameraTest::OnGuiRenderer()
{
	glm::vec3 camera = FreeCamera->GetEyePosition();
	glm::vec2 rotation = FreeCamera->GetPY();
	float zoom = FreeCamera->GetCameraFOV();
	float dis = FreeCamera->GetDistance();
	ImGui::Begin("Free Camera");
	ImGui::SliderFloat3("position", &camera.x, -20.0f,20.0f);
	ImGui::SliderFloat2("pitch/yaw", &rotation.x, -20.0f,20.0f);
	ImGui::SliderFloat("FOV", &zoom, 1.0f, 90.0f);
	ImGui::SliderFloat("distance", &dis, 1.0f, 90.0f);

	ImGui::ColorEdit3("Ambient", &m_MyMaterials.ambient.x);
	ImGui::ColorEdit3("Diffuse", &m_MyMaterials.diffuse.x);
	ImGui::ColorEdit3("Specular", &m_MyMaterials.specular.x);
	ImGui::SliderFloat("shininess", &m_MyMaterials.shininess, .0f, 1.0f);
	ImGui::SliderFloat("cube distance", &cube_offset, -30.0f, 30.0f);
	ImGui::SliderAngle("spot radius", &inneRadius, 5.0f, 20.0f);
	ImGui::End();

	ImGui::Begin("Shading Inspector");
	ImGui::RadioButton("Phong", &m_shaderActive, 0);
	ImGui::SameLine();
	ImGui::RadioButton("Gouraud", &m_shaderActive, 1);
	ImGui::SameLine();
	ImGui::RadioButton("NormalMap", &m_shaderActive, 2);
	ImGui::SameLine();
	ImGui::RadioButton("Toon", &m_shaderActive, 3);
	ImGui::End();


	outerRadius = inneRadius + 5.0f;
}

void test::CameraTest::BindSelectedShader(LIGHT_MODELS& option)
{
	if (option == LIGHT_MODELS::PHONG)
	{
		m_lightModelShaders[0].Bind();
		UpdateScene(&m_lightModelShaders[0]);
	}
	else if (option == LIGHT_MODELS::GOURAUD)
	{
		m_lightModelShaders[1].Bind();
		UpdateScene(&m_lightModelShaders[1]);
	}
	else if (option == LIGHT_MODELS::NORMAL_MAP)
	{
		m_lightModelShaders[2].Bind();
		UpdateScene(&m_lightModelShaders[2]);
	}
	else if (option == LIGHT_MODELS::TOON)
	{
		m_lightModelShaders[3].Bind();
		UpdateScene(&m_lightModelShaders[3]);
	}
}


void test::CameraTest::UpdateScene(Shader* shader)
{
	glm::mat4 trans  = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
	//glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f) * float(glfwGetTime()), glm::vec3(.20f, 0.30f, .40f));
	glm::mat4 rotate = glm::mat4(1.0f);
	glm::mat4 scale  = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	glm::mat4 model  = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
	model *= trans * rotate * scale;
	
	glm::mat4 proy = glm::perspective(glm::radians(FreeCamera->GetCameraFOV()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	glm::mat4 mvp = proy * m_viewMatrix * model;


	//materials
	shader->SetUniform3f("u_material.ka", m_MyMaterials.ambient.x, m_MyMaterials.ambient.y, m_MyMaterials.ambient.z);
	shader->SetUniform3f("u_material.kd", m_MyMaterials.diffuse.x, m_MyMaterials.diffuse.y, m_MyMaterials.diffuse.z);
	shader->SetUniform3f("u_material.ks", m_MyMaterials.specular.x, m_MyMaterials.specular.y, m_MyMaterials.specular.z);
	shader->SetUniform1f("u_material.sh", m_MyMaterials.shininess);	
	//vectors
	shader->SetUniform3f("u_colorBase", 1.0f, 1.0f, 1.0f);
	for (int i = 0; i < lightCubePositions.size(); i++)
	{
		std::string n = std::to_string(i);
		glm::vec3 pos = lightCubePositions[i] + glm::vec3(cube_offset, 0.0f, cube_offset);
		shader->SetUniform3f("u_pointLights[" + n + "].origin", pos.x, pos.y, pos.z);
		shader->SetUniform3f("u_pointLights[" + n + "].color", lightCubeColors[i].r, lightCubeColors[i].g, lightCubeColors[i].b);
		shader->SetUniform1f("u_pointLights[" + n + "].phi", glm::cos(inneRadius));
		shader->SetUniform1f("u_pointLights[" + n + "].gamma", glm::cos(outerRadius));
	}
	shader->SetUniform3f("u_cameraPosition", FreeCamera->GetEyePosition().x, FreeCamera->GetEyePosition().y, FreeCamera->GetEyePosition().z);
	shader->SetUniform3f("u_attConst", att.x,att.y, att.z);
	//matrices
	shader->SetUniformMat4f("u_mvp", mvp);
	shader->SetUniformMat4f("u_model", model);
	shader->SetUniformMat4f("u_view", m_viewMatrix);
	

	
}
