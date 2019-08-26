#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<chrono>

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
//DEBUGGER 
#define ASSERT(x) if(!(x)) __debugbreak();
#ifdef _DEBUG
#define GLCALL(x) ClearErrors();x;ASSERT(LogCallErrors(#x, __FILE__, __LINE__))
#else 
#define GLCALL(x) x
#endif //_DEBUG


static void ClearErrors()
{//clean error buffer
	while (glGetError() != GL_NO_ERROR);
}

static bool LogCallErrors(const char* functionName, const char* filename, int line)
{
	while (GLenum error_code = glGetError())
	{
		std::cout << "[OPENGL ERROR] (" << std::hex<< error_code <<std::dec << ")\nfunName: " << functionName <<"\nfile:"<< filename 
			<<" : " <<line << std::endl;
		return false;//not succesfull
	}
	return true;
}


static std::pair<std::string, std::string> ParseShader(const std::string& filepath)
{
	std::ifstream inputFile;
	inputFile.open(filepath);
	if (inputFile.fail())
	{
		perror("ERROR CANNOT OPEN SHADER FILE");
		return { 0, 0 };
	}

	//read the file, change mode, send data corresponding to mode
	enum class ShaderType{ NONE =-1, VERTEX, FRAGMENT};
	std::string grabber;
	std::stringstream stream[2];
	ShaderType Type = ShaderType::NONE;//default

	while (getline(inputFile, grabber))
	{
		//IF finds #shader change the type
		if (grabber.find("#shader") != std::string::npos)
		{
			//select shader type from file
			if (grabber.find("vertex") != std::string::npos)		 Type = ShaderType::VERTEX;
			else if (grabber.find("fragment") != std::string::npos) Type = ShaderType::FRAGMENT;
		}
			//load the stream base on the set type
		else stream[(int)Type] << grabber << "\n";
	}
	inputFile.close();

	return { stream[(int)ShaderType::VERTEX].str(), stream[(int)ShaderType::FRAGMENT].str() };
}

static unsigned int CompileShader(const std::string& source, unsigned int type)
{

	//Holds the memory of the data need to create a shader object 
	unsigned int id = glCreateShader(type);

	//get the pointer of the shader file
	const char* src = &source[0];



	//preparing the shader object
	glShaderSource(id, 1, &src, nullptr);
	//compile the string inside the shader object
	glCompileShader(id);

	//error handler
	int error;
	glGetShaderiv(id, GL_COMPILE_STATUS, &error);
	if (!error)
	{
		int error_lenght;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &error_lenght);

		//char* outMessage = (char*)alloca(sizeof(char) * error_lenght);
		char* outMessage = new char[error_lenght];
		glGetShaderInfoLog(id, error_lenght, &error_lenght, outMessage);

		std::cout << "ERROR!!, Failed To Compile " << ((type == GL_VERTEX_SHADER) ? "VERTEX_SHADER\n" : "FRAG_SHADER\n")
			<< outMessage << std::endl;
		glDeleteShader(id);

		delete[] outMessage;
		//for debuging return 0
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	//prepares this id to holds all data about the shader pipeline
	unsigned int program = glCreateProgram();

	//starts prepares and return the id of the shader compilation base on the type and string data
	unsigned int vertex_shader = CompileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int frag_shader = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

	//unify all shader object data inside one program before send it to GPU
	glAttachShader(program, vertex_shader);
	glAttachShader(program, frag_shader);

	//validates and creates an executable for the GPU
	glLinkProgram(program);
	glValidateProgram(program);

	//after finnish, deletes all intermediates
	glDeleteShader(vertex_shader);
	glDeleteShader(frag_shader);
	
	return program;
}


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

	float tri_coord[] =
	{
		-0.9f,  0.9f,//0
		-0.9f, 0.0f,//1
		0.0f,  0.9f,//2
		//second tri
		0.0f,  0.0f,//3
	};
	unsigned int indices[] =
	{
		0,1,2,//top triangle
		3,2,1//down triangle
	};

	/*Core OpenGL requires that we use a VAO so it knows what to do with our vertex inputs.
	If we fail to bind a VAO, OpenGL will most likely refuse to draw anything.*/
	unsigned int vao;

	GLCALL(glGenVertexArrays(1, &vao));
	GLCALL(glBindVertexArray(vao));



	//generates an ID for vertex buffer object 
	unsigned int tri_buffer;
	glGenBuffers(1, &tri_buffer);
	std::cout << "triangle id: " << tri_buffer << std::endl;

	//select the buffer to work with
	glBindBuffer(GL_ARRAY_BUFFER, tri_buffer);
	//send the data to the buffer
	glBufferData(GL_ARRAY_BUFFER, 4*2* sizeof(float), tri_coord, GL_STATIC_DRAW);


	//tell opengl how to work the specify buffer layout. ALWAYS HAS TO BE SET BEFORE DRAWING A BUFFER OBJECT 
	GLCALL(glEnableVertexAttribArray(0));
	//This function links the tri_buffer and his attributes (indices_buffer) with the current bound VAO
	GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (void*)0));

	//same way, the indices or elements needs a buffer to work, so generate a index buffer
	unsigned int index_buffer_obj;
	glGenBuffers(1, &index_buffer_obj);
	std::cout << "ibo id: " << index_buffer_obj<< std::endl;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_obj);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6* sizeof(int), indices, GL_STATIC_DRAW);

	//shader code HELL YEAH!
	
	//c++17 structured binding	
	auto [VertexShader, FragShader] = ParseShader("shaders/Base.shader");

	//start all the shader data process
	unsigned int shaderProgram = CreateShader(VertexShader, FragShader);

	//bind all the shader to OPENGL
	glUseProgram(shaderProgram);




	//to set uniform first you have to bound a shader before
	unsigned int location = glGetUniformLocation(shaderProgram, "u_color");
	ASSERT(location != -1)
	
	//colors
	float r = 1.0f,g=1.0f, b = 1.0f;
	float addRed = 0.05f;
	float addGreen = 0.01f;
	float addBlue = 0.025f;
	float alfa = 1.0f;
	//smoth
	auto t1 = std::chrono::system_clock::now();
	auto t2 = std::chrono::system_clock::now();


	//UNBOUND EVERYTHING
	GLCALL(glBindVertexArray(0));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCALL(glUseProgram(0));
	
	while (!glfwWindowShouldClose(mainWindow))
	{
		//update clock
		t2= std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = t2 - t1;
		//time per frame coefficient
		float deltaTime = elapsedTime.count();

		//update colors base on framerate
		if		(r > 1.0f) addRed = -0.05f;
		else if	(r < 0.0f) addRed = 0.05f;
		
		if	   (g > 1.0f) addGreen= -0.01f;
		else if (g < 0.0f)addGreen = 0.01f;

		if	   (b > 1.0f) addBlue = -0.025f;
		else if (b < 0.0f)addBlue = 0.025f;

		if (deltaTime > 1.0f)
			deltaTime = 0.10f;
		
		r += addRed * deltaTime;
		g += addGreen * deltaTime;
		b += addBlue * deltaTime;

		//CLEAR SCREEN
		glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//shaders binding and uniform sending data
		GLCALL(glUseProgram(shaderProgram));
		GLCALL(glUniform4f(location, r, g, b, alfa));
		
		//the benefit of VAO are that we dont need to bind the buffer object or their attributes again, only their indices
		GLCALL(glBindVertexArray(vao));
		
		GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0));
		
		//swap buffers before draw 
		glfwSwapBuffers(mainWindow);
		//event manager
		glfwPollEvents();
		glfwSetKeyCallback(mainWindow, keyCallBack);
		
		t2 = t1;
	}

	//clean all data related to the shader
	glDeleteProgram(shaderProgram);
	
	glfwTerminate();
	return EXIT_SUCCESS;
}