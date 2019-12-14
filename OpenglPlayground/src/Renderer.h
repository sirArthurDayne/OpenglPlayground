#pragma once
#include<GL/glew.h>
#include"VertexArray.h"
#include"IndexBuffer.h"

#define WIDTH 1080
#define HEIGHT 720

//DEBUGGER 
#define ASSERT(x) if(!(x)) __debugbreak();
#ifdef _DEBUG
#define GLCALL(x) ClearErrors();x;ASSERT(LogCallErrors(#x, __FILE__, __LINE__))
#else 
#define GLCALL(x) x
#endif //_DEBUG


void ClearErrors();
bool LogCallErrors(const char* functionName, const char* filename, int line);

class Renderer
{
public:
	Renderer();
	void Clear(const float r, const float g, const float b, float a = 1.0f);
	void Draw(const VertexArray* vao, const IndexBuffer* ibo) const;
	~Renderer();
};