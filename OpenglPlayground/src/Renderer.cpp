#include"Renderer.h"
#include<iostream>

void ClearErrors()
{//clean error buffer
	while (glGetError() != GL_NO_ERROR);
}

bool LogCallErrors(const char* functionName, const char* filename, int line)
{
	while (GLenum error_code = glGetError())
	{
		std::cout << "[OPENGL ERROR] (" << std::hex << error_code << std::dec << ")\nfunName: " << functionName << "\nfile:" << filename
			<< " : " << line << std::endl;
		return false;//not succesfull
	}
	return true;
}
