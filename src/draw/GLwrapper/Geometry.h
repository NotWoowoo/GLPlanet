#pragma once
#include <initializer_list>
#include <glad\glad.h>

class Geometry
{
public:
	
	Geometry();
	~Geometry();

	void vertexData(GLsizeiptr dataSize, const void* data, bool dynamicData = false);
	void indexData(GLsizeiptr dataSize, const void* data, bool dynamicData = false);
	void vertexLayout(GLuint index, GLint count, GLenum type, GLsizei stride, const void* offset);

	void bind();
	
private:
	GLuint VBO = 0, VAO = 0, IBO = 0;
};

