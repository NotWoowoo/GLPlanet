#pragma once

#include <string>
#include <unordered_map>
#include <glm\glm.hpp>

class Shader
{
public:
	Shader(std::string vertexSource, std::string fragmentSource);

	~Shader();

	void use();


	void uniform(std::string name, float f);
	void uniform(std::string name, glm::mat4 mat);

private:
	unsigned int program;
	std::unordered_map<std::string, int> uniformLocations;

	void generateProgram(unsigned int vertexSource, unsigned int fragmentSource);
	static unsigned int compileShader(std::string src, unsigned int type);

	int getUniformLocation(std::string name);
};

