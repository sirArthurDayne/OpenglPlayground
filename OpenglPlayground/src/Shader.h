#pragma once
#include<iostream>
#include<string>
#include<unordered_map>
class Shader
{
public:
	Shader(const std::string& filepath);

	void Bind() const;
	void Unbind() const;
	void SetUniform4f(const std::string& name, float* data);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform2f(const std::string& name, float data, float data2);
private:
	std::pair<std::string, std::string> ParseShader(const std::string& filepath);
	void CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(const std::string& source, unsigned int type);
	int GetUniformLocation(const std::string& name);
public:
	~Shader();
private:
	unsigned int m_rendererID;
	std::unordered_map<std::string, int> m_locationsCache;
	std::string m_filePath;
};

