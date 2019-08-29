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


Renderer::Renderer()
{
}

void Renderer::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray* vao, const IndexBuffer* ibo) const
{
	vao->Bind();
	GLCALL(glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, (void*)0));
}

Renderer::~Renderer()
{

}

