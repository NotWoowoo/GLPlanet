#include <glad\glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

Shader::Shader(std::string vertexSource, std::string fragmentSource) {
	unsigned int vShader = compileShader(vertexSource, GL_VERTEX_SHADER);
	unsigned int fShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
	generateProgram(vShader, fShader);
}

Shader::~Shader() {
	glDeleteProgram(program);
}

void Shader::use() {
	glUseProgram(program);
}

void Shader::generateProgram(unsigned int vShader, unsigned int fShader) {
	program = glCreateProgram();

	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	glLinkProgram(program);
	glValidateProgram(program);

	int programValidationStatus;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &programValidationStatus);

	if (programValidationStatus == GL_FALSE) {
		char programInfoLog[1024];
		glGetProgramInfoLog(program, sizeof programInfoLog, nullptr, programInfoLog);
		printf("[SHADER PROGRAM ERROR] %s\n", programInfoLog);
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
}

static std::string srcCommon = "#version 330 core\n";

unsigned int Shader::compileShader(std::string src, unsigned int type) {
	unsigned int shader = glCreateShader(type);

	const char* srcArr[] = { srcCommon.c_str(), src.c_str() };
	const int srcLengths[] = { srcCommon.length(), src.length() };
	glShaderSource(shader, sizeof(srcArr)/sizeof(char *), srcArr, srcLengths);
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		char message[1024];
		glGetShaderInfoLog(shader, sizeof message, nullptr, message);
		printf("[SHADER ERROR] %s\n", message);
	}

	return shader;
}

int Shader::getUniformLocation(std::string name) {
	if (uniformLocations.find(name) == uniformLocations.end())
		uniformLocations[name] = glGetUniformLocation(program, name.c_str());

	return uniformLocations[name];
}

void Shader::uniform(std::string name, float f) {
	glUniform1f(getUniformLocation(name), f);
}

void Shader::uniform(std::string name, glm::mat4 mat) {
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}