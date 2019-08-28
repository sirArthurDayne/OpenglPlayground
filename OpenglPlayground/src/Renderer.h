#pragma once
#include<GL/glew.h>
//DEBUGGER 
#define ASSERT(x) if(!(x)) __debugbreak();
#ifdef _DEBUG
#define GLCALL(x) ClearErrors();x;ASSERT(LogCallErrors(#x, __FILE__, __LINE__))
#else 
#define GLCALL(x) x
#endif //_DEBUG


void ClearErrors();
bool LogCallErrors(const char* functionName, const char* filename, int line);



