#include <vector>
#include "Geometry.h"

Geometry::Geometry(){
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
}

void Geometry::vertexData(GLsizeiptr dataSize, const void* data, bool dynamicData) {
	//glBindVertexArray(VAO);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, dynamicData ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

void Geometry::indexData(GLsizeiptr dataSize, const void* data, bool dynamicData) {
	//glBindVertexArray(VAO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, dynamicData ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

void Geometry::vertexLayout(GLuint index, GLint count, GLenum type, GLsizei stride, const void* offset) {
	//glBindVertexArray(VAO);
	glVertexAttribPointer(index, count, type, false, stride, offset);
	glEnableVertexAttribArray(index);
}

Geometry::~Geometry() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Geometry::bind() {
	glBindVertexArray(VAO);
}