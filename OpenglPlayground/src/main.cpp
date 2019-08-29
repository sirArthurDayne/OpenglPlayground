#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<chrono>
#include"Renderer.h"
#include"VertexArray.h"
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"Shader.h"
/*OPENGL--> is an implementation that runs & works on your GPU

OPENGL works as a state machine.

example for drawing a triangle:

1. generate a buffer
1.1 load data(anticlockwise)
1.2 generate an index buffer
2. select a shader 
3. draw a triangle with that data.
*/
int WIDTH = 1080;
int HEIGHT = 720;


static void changeBufferSizeCallBack(GLFWwindow* win, int width, int height)
{
	glViewport(0, 0, width, height);
}


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


int main(void)
{
	GLFWwindow* mainWindow;
	//if doesnt work
	if (!glfwInit())
		return EXIT_FAILURE;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,4);
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

	float tri_pos[] =
	{
		-1.0f,  1.0f,//0
		 0.0f, 0.0f,//1
		 1.0f,  1.0f,//2
		-1.0f, -1.0f,//3
		 1.0f, -1.0f,//4
	};
	unsigned int tri_indices[] =
	{
		0,1,2,//top triangle
		3,1,4,//down triangle
	};

	/*Core OpenGL requires that we use a VAO so it knows what to do with our vertex inputs.
	If we fail to bind a VAO, OpenGL will most likely refuse to draw anything.*/
	//VAO stores all of the state needed to supply vertex data
	VertexArray* vao = new VertexArray();
	VertexLayout* vl = new VertexLayout();
	vl->Push<float>(2);
	//generates an ID for vertex buffer object 
	VertexBuffer* vertexBuffer = new VertexBuffer(tri_pos, 5 * 2 * sizeof(float));
	vao->AddBuffer(*vertexBuffer, *vl);

	//same way, the indices or elements needs a buffer to work, so generate a index buffer
	IndexBuffer* indexBuffer = new IndexBuffer(tri_indices, 6);
	
	//shader code HELL YEAH!
	//c++17 structured binding	
	Shader* tri_shader = new Shader("shaders/Base.shader");

	//colors
	float* tri_rgba = new float[4];
	tri_rgba[0] = 1.0f;
	tri_rgba[1] = 1.0f;
	tri_rgba[2] = 1.0f;
	tri_rgba[3] = 1.0f;
	
	float addRed = 0.05f;
	float addGreen = 0.01f;
	float addBlue = 0.025f;
	
	//smoth
	auto t1 = std::chrono::system_clock::now();
	auto t2 = std::chrono::system_clock::now();


	//UNBOUND EVERYTHING
	vao->Unbind();
	vertexBuffer->Unbind();
	indexBuffer->Unbind();
	tri_shader->Unbind();
	
	while (!glfwWindowShouldClose(mainWindow))
	{
		//update clock
		t2= std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = t2 - t1;
		//time per frame coefficient
		float deltaTime = elapsedTime.count();

		//update colors base on framerate
		if		(tri_rgba[0] > 1.0f) addRed = -0.05f;
		else if	(tri_rgba[0] < 0.0f) addRed = 0.05f;
		
		if	   (tri_rgba[1] > 1.0f) addGreen= -0.01f;
		else if (tri_rgba[1] < 0.0f)addGreen = 0.01f;

		if	   (tri_rgba[2] > 1.0f) addBlue = -0.025f;
		else if (tri_rgba[2] < 0.0f)addBlue = 0.025f;

		if (deltaTime > 1.0f)
			deltaTime = 0.10f;
		
		tri_rgba[0] += addRed * deltaTime;
		tri_rgba[1] += addGreen * deltaTime;
		tri_rgba[2] += addBlue * deltaTime;

		//CLEAR SCREEN
		glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//shaders binding and uniform sending data
		tri_shader->Bind();
		tri_shader->SetUniform4f("u_color", tri_rgba);
		
		//the benefit of VAO are that we dont need to bind the buffer object or their attributes again
		vao->Bind();
		GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0));
		
		//swap buffers before draw 
		glfwSwapBuffers(mainWindow);
		//event manager
		glfwPollEvents();
		glfwSetKeyCallback(mainWindow, keyCallBack);
		
		t2 = t1;
	}
	delete vertexBuffer;
	delete indexBuffer;
	delete vl;
	delete vao;
	delete tri_shader;
	glfwTerminate();
	return EXIT_SUCCESS;
}