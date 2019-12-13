#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<chrono>
#include"Renderer.h"
#include"VertexArray.h"
#include"VertexLayout.h"
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"Shader.h"
#include"Texture.h"

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


/*openGL--> is an implementation that runs & works on your GPU
openGL works as a state machine.

example for drawing a triangle:

1. generate a buffer
1.1 load data(anticlockwise)
1.2 generate an index buffer
2. select a shader 
3. draw a triangle with that data.
*/
int WIDTH = 1080;
int HEIGHT = 720;


static void keyCallBack(GLFWwindow* win, int key, int scanCode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(win, GL_TRUE);
	//change render mode
	if (key ==  GLFW_KEY_1 && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (key ==  GLFW_KEY_2 && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

static void mousecallBack(GLFWwindow*  win, double& x, double& y)
{
	GLCALL(glfwGetCursorPos(win, &x, &y));
}




int main(void)
{
	GLFWwindow* mainWindow;
	//if doesnt work
	if (!glfwInit())
		return EXIT_FAILURE;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Practica con OpenGL", NULL, NULL);

	if (!mainWindow)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(mainWindow);

	if (glewInit() != GLEW_OK) std::cout << "GLEW FAILED\n";
	std::cout << glGetString(GL_VERSION) << std::endl;


	//BLENDING FOR TEXTURING
	/*ALPHA ECUATION
	 * out_alpha = src_alpha + dest_alpha * (1 - src_alpha)
	   If dest_alpha equals 1, then out_alpha also equals 1
	 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	float tri_pos[] =
	{
		//POSITION		//TEXTURE
		100.0f, -100.0f,		1.0f, 0.0f,//0
		100.0f, 100.0f,		1.0f, 1.0f,//1
		-100.0f, -100.0f,		0.0f, 0.0f,//2
		-100.0f, 100.0f,		0.0f, 1.0f//3
	};
	unsigned int tri_indices[] =
	{
		2,0,1,//top triangle
		3,2,1,//down triangle
	};

	glm::mat4 proy = glm::ortho(0.0f, float(WIDTH), 0.0f, float(HEIGHT), -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0));
	glm::vec3 translationA(0.0f, 0.0f, 0.0f);
	glm::vec3 translationB(220.0f, 110.0f, 0.0f);
	
	/*Core OpenGL requires that we use a VAO so it knows what to do with our vertex inputs.
	If we fail to bind a VAO, OpenGL will most likely refuse to draw anything.*/
	//VAO stores all of the state needed to supply vertex data
	VertexArray* vao = new VertexArray();
	VertexLayout* vl = new VertexLayout();
	vl->Push<float>(2);//pos
	vl->Push<float>(2);//texture
	//generates an ID for vertex buffer object 
	VertexBuffer* vertexBuffer = new VertexBuffer(tri_pos, 4 * 4 * sizeof(float));
	vao->AddBuffer(*vertexBuffer, *vl);

	//same way, the indices or elements needs a buffer to work, so generate a index buffer
	IndexBuffer* indexBuffer = new IndexBuffer(tri_indices, 6);
	
	//shader code HELL YEAH!
	//c++17 structured binding	
	Shader* tri_shader = new Shader("shaders/Base.shader");
	tri_shader->SetUniform1i("u_texture", 0);
	
	//colors %
	float* rgba = new float[4];
	rgba[0] = 1.0f;
	rgba[1] = 1.0f;
	rgba[2] = 1.0f;
	rgba[3] = 1.0f;//alpha
	
	//smoth
	auto t1 = std::chrono::system_clock::now();
	auto t2 = std::chrono::system_clock::now();

	//TEXTURES
	Texture* texture = new Texture("character.png");
	texture->Bind();
	
	//UNBOUND EVERYTHING
	vao->Unbind();
	vertexBuffer->Unbind();
	indexBuffer->Unbind();
	tri_shader->Unbind();
	
	
	
	
	double mouseX = 0.0f, mouseY = 0.0f;
	//RENDERER
	Renderer renderer;


	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	while (!glfwWindowShouldClose(mainWindow))
	{
		//update clock
		t2= std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = t2 - t1;
		//time per frame coefficient
		const float delta_time = elapsedTime.count();
		
		renderer.Clear(0.0f, 0.0f, 0.0f);
		
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		ImGui::Begin("Move Characters!");
		ImGui::SliderFloat3("coordenadas A", &translationA.x, 0.0f, float(WIDTH));
		ImGui::SliderFloat3("coordenadas B", &translationB.x, 0.0f, float(WIDTH));
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
		
		//shader binding and uniform sending data
		tri_shader->Bind();
		tri_shader->SetUniform1f("u_time", delta_time);
		tri_shader->SetUniform2f("u_resolution", (float)WIDTH, (float)HEIGHT);
		tri_shader->SetUniform2f("u_mouse", (float)mouseX, (float)mouseY);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
		glm::mat4 mvp = proy * view * model;
		tri_shader->SetUniformMat4f("u_mvp", mvp);
		renderer.Draw(vao, indexBuffer);
		//second draw call
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
			glm::mat4 mvp = proy * view * model;
			tri_shader->SetUniformMat4f("u_mvp", mvp);
			renderer.Draw(vao, indexBuffer);
		}
		
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		//swap buffers before draw 
		glfwSwapBuffers(mainWindow);
		//event manager
		glfwPollEvents();
		glfwSetKeyCallback(mainWindow, keyCallBack);
		mousecallBack(mainWindow, mouseX, mouseY);
		t2 = t1;
	}
	delete vertexBuffer;
	delete indexBuffer;
	delete vl;
	delete vao;
	delete tri_shader;
	delete texture;

	// Cleanup Imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
	return EXIT_SUCCESS;
}