#include <vector>
#include <glad\glad.h>
#include "Geometry.h"

static struct TypeInfo {
	GLenum type;
	int size;
	int count;
};

static TypeInfo getTypeInfo(Geometry::Format type) {
	switch (type) {
		case Geometry::fmtFloat: return { GL_FLOAT,          sizeof(GLfloat), 1 };
		case Geometry::fmtUint:  return { GL_UNSIGNED_INT,   sizeof(GLuint),  1 };
		case Geometry::fmtVec2f: return { GL_FLOAT,        2*sizeof(GLfloat), 2 };
		case Geometry::fmtVec3f: return { GL_FLOAT,        3*sizeof(GLfloat), 3 };
	}
}

Geometry::Geometry(std::initializer_list<Format> format, bool dynamicDraw, int dataSize){
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, dataSize, nullptr, dynamicDraw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	int stride = 0;
	std::vector<TypeInfo> formatTypes;
	for (Format f : format) {
		TypeInfo t = getTypeInfo(f);
		formatTypes.push_back(t);
		stride += t.size;
	}

	int runningOffset = 0;
	for (int i = 0; i < formatTypes.size(); ++i) {
		TypeInfo t = formatTypes[i];
		glVertexAttribPointer(i, t.count, t.type, false, stride, (void *)runningOffset);
		glEnableVertexAttribArray(i);
		runningOffset += t.size;
	}
}

Geometry::~Geometry() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Geometry::bind() {
	glBindVertexArray(VAO);
}