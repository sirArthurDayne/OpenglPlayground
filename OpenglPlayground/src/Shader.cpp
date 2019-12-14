#include "Shader.h"
#include"Renderer.h"
#include<fstream>
#include<sstream>
#include<iostream>

Shader::Shader(const std::string& filepath): m_filePath(filepath), m_rendererID(0)
{
	//c++17 structured binding	
	//getting data from file
	auto[VertexShader, FragmentShader] = ParseShader(filepath);
	
	//create program
	GLCALL(CreateShader(VertexShader, FragmentShader));

	//bind all the shader to openGL
	Bind();
}

void Shader::Bind() const
{
	GLCALL(glUseProgram(m_rendererID));
}

void Shader::Unbind() const
{
	GLCALL(glUseProgram(0));
}

std::pair<std::string, std::string> Shader::ParseShader(const std::string& filepath)
{
	std::ifstream inputFile;
	inputFile.open(filepath);
	if (inputFile.fail())
	{
		perror("ERROR CANNOT OPEN SHADER FILE");
		return { };
	}

	//read the file, change mode, send data corresponding to mode
	enum class ShaderType { NONE = -1, VERTEX, FRAGMENT };
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

void Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	//prepares this id to holds all data about the shader pipeline
	m_rendererID = glCreateProgram();

	//starts prepares and return the id of the shader compilation base on the type and string data
	const unsigned int vertex_shader = CompileShader(vertexShader, GL_VERTEX_SHADER);
	const unsigned int frag_shader = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

	//unify all shader object data inside one program before send it to GPU
	GLCALL(glAttachShader(m_rendererID, vertex_shader));
	GLCALL(glAttachShader(m_rendererID, frag_shader));

	//validates and creates an executable for the GPU
	GLCALL(glLinkProgram(m_rendererID));
	GLCALL(glValidateProgram(m_rendererID));

	//after finnish, deletes all intermediates
	GLCALL(glDeleteShader(vertex_shader));
	GLCALL(glDeleteShader(frag_shader));
}

void Shader::SetUniform4f(const std::string& name, float* data)
{
	//get & set location for uniform 
	GLCALL(glUniform4f(GetUniformLocation(name), *data, *(data + 1), *(data + 2), *(data + 3) ));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform2f(const std::string& name, float data, float data2)
{
	glUniform2f(GetUniformLocation(name), data, data2);
}


void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& data)
{
	GLCALL(glUniformMatrix4fv(GetUniformLocation(name),1, GL_FALSE, &data[0][0]));
}

unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
	//Holds the memory of the data need to create a shader object 
	const unsigned int typeID = glCreateShader(type);

	//get the pointer of the shader file
	const char* src = &source[0];

	//preparing the shader object
	GLCALL(glShaderSource(typeID, 1, &src, nullptr));
	
	//compile the string inside the shader object
	GLCALL(glCompileShader(typeID));

	//error handler
	int error;
	
	glGetShaderiv(typeID, GL_COMPILE_STATUS, &error);
	
	if (!error)
	{
		int error_length;
		glGetShaderiv(typeID, GL_INFO_LOG_LENGTH, &error_length);

		//char* outMessage = (char*)alloca(sizeof(char) * error_lenght);
		char* outMessage = new char[error_length];
		glGetShaderInfoLog(typeID, error_length, &error_length, outMessage);

		std::cout << "ERROR!!, Failed To Compile " << ((type == GL_VERTEX_SHADER) ? "VERTEX_SHADER\n" : "FRAG_SHADER\n")
			<< outMessage << std::endl;
		glDeleteShader(typeID);

		delete[] outMessage;
		return 0;
	}

	return typeID;	
}

int Shader::GetUniformLocation(const std::string& name) const
{
	if (m_locationsCache.find(name) != m_locationsCache.end()) return m_locationsCache[name];

	//to set uniform first you have to bound a shader before
	GLCALL(const int location = glGetUniformLocation(m_rendererID, name.c_str()));

	if (location == -1)
		std::cout << "WARNING uniform '" << name << "' doesn't exit\n";
	m_locationsCache[name] = location;

	return location;
}

Shader::~Shader()
{
	Unbind();
}
