#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<glm.hpp>
#include<iostream>
#include<chrono>
#include"Renderer.h"
#include"VertexArray.h"
#include"VertexLayout.h"
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"Shader.h"
#include"Texture.h"
/*openGL--> is an implementation that runs & works on your GPU
openGL works as a state machine.

example for drawing a triangle:

1. generate a buffer
1.1 load data(anticlockwise)
1.2 generate an index buffer
2. select a shader 
3. draw a triangle with that data.
*/
int WIDTH = 1920;
int HEIGHT = 1080;


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

	float tri_pos[] =
	{
		//POSITION			//TEXTURE
		-1.0f, 1.0f,			0.0f, 1.0f,//0
		-1.0f, -1.0f,		0.0f, 0.0f,//1
		 1.0f,  1.0f,		1.0f, 1.0f,//2
		 1.0f,  -1.0f,		1.0f, 0.0f//3
	};
	unsigned int tri_indices[] =
	{
		2,0,1,//top triangle
		3,2,1,//down triangle
	};

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
	Texture* texture = new Texture("piratelogo.png");
	texture->Bind();
	
	//UNBOUND EVERYTHING
	vao->Unbind();
	vertexBuffer->Unbind();
	indexBuffer->Unbind();
	tri_shader->Unbind();
	
	
	//BLENDING FOR TEXTURING
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	/*ALPHA ECUATION
	 * out_alpha = src_alpha + dest_alpha * (1 - src_alpha)
	   If dest_alpha equals 1, then out_alpha also equals 1
	 */
	double mouseX = 0.0f, mouseY = 0.0f;
	//RENDERER
	Renderer renderer;
	while (!glfwWindowShouldClose(mainWindow))
	{
		//update clock
		t2= std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = t2 - t1;
		//time per frame coefficient
		const float delta_time = elapsedTime.count();
		
		renderer.Clear(0.40f, 0.0f, 0.3f);
		
		//shader binding and uniform sending data
		tri_shader->Bind();
		tri_shader->SetUniform1f("u_time", delta_time);
		tri_shader->SetUniform4f("u_colorBase", rgba);
		tri_shader->SetUniform2f("u_resolution", (float)WIDTH, (float)HEIGHT);
		tri_shader->SetUniform2f("u_mouse", (float)mouseX, (float)mouseY);
		renderer.Draw(vao, indexBuffer);
		
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
	
	glfwTerminate();
	return EXIT_SUCCESS;
}