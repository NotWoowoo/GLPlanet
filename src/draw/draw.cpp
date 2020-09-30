#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <draw/draw.h>
#include <draw/GLwrapper/Shader.h>
#include <draw/Camera.h>
#include <draw/GLwrapper/Geometry.h>

#include "window.h"

Shader* s;
Camera* c;
Geometry* g;

void Draw::init() {
	glEnable(GL_DEPTH_TEST);

	float data[] = {
		-1, -1, 0,
		1, -1, 0,
		1, 1, 0,
		-1, 1, 0
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	g = new Geometry();
	g->indexData(sizeof(indices), indices);
	g->vertexData(sizeof(data), data);
	g->vertexLayout(0, 3, GL_FLOAT, sizeof(float) * 3, 0);

	//vData->begin(bool generateIndices)
	//vData->vertexF(floats)
	//vData->vertexFI(floats, ints)
	//vData->vertexI(ints)
	//vData->face(index1, 2, 3)
	//vData->face(vector1, 2, 3, ...)
	//vData->end()   maybe


	std::string vSrc =
		"layout (location = 0) in vec3 inPos;\n"
		"out vec3 v_pos;\n"
		"out float v_instanceId;\n"
		"uniform mat4 MVP;\n"
		"float scale = 500;\n"
		"void main(){\n"
		"v_instanceId = gl_InstanceID;\n"
		"v_pos = inPos;\n"
		"vec3 tPos = vec3(inPos.x+scale*sin(gl_InstanceID/100.4), inPos.y+scale*cos(gl_InstanceID/50.3), inPos.z-scale*sin(gl_InstanceID/30.3));\n"
		"gl_Position = MVP*vec4(tPos, 1.0);\n"
		"}";

	std::string fSrc =
		"layout (location = 0) out vec4 outCol;"
		"in vec3 v_pos;\n"
		"in float v_instanceId;\n"
		"void main(){\n"
		"vec3 tCol = abs(sin(v_pos));\n"
		"outCol = vec4(v_pos+0.3+abs(sin(v_instanceId/4.0)), 1.0);\n"
		"}";
	
	s = new Shader(vSrc, fSrc);
	s->use();

	c = new Camera();

	Window::setRelativeMouseMode(true);
	Window::keyboardCallback = []() {
		if (window_key(ESCAPE))
			Window::shouldClose = true;

		if (window_key(LALT))
			Window::setRelativeMouseMode(!Window::isRelativeMouseMode());

		if (window_key(Z)) {
			c->setOrientation(glm::radians(180.f), 0.f, 0.f);
			c->setPosition(0, 0, 0);
		}

		if (window_key(X)) {
			c->setOrientation(0.f, 0.f, 0.f);
			c->setPosition(0, 0, -10);
		}
	};
}

void Draw::update() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (window_key(W))
		c->moveForward(0.3f);

	if (window_key(S))
		c->moveForward(-0.3f);

	if (window_key(D))
		c->moveRight(0.3f);

	if (window_key(A))
		c->moveRight(-0.3f);

	if (window_key(E))
		c->moveUp(0.3f);

	if (window_key(Q))
		c->moveUp(-0.3f);

	if (window_key(R))
		c->addOrientation(0, 0, 0.01);

	if (window_key(F))
		c->addOrientation(0, 0, -0.01);

	if(Window::isRelativeMouseMode())
		c->pan(glm::radians((float)Window::mouseDX), -glm::radians((float)Window::mouseDY));

	glm::mat4 m(1.0f);
	m *= glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 10000.0f);
	m *= c->getLookatMatrix();
	m = glm::translate(m, glm::vec3(0.0f, 0.0f, -5.0f));
	s->uniform("MVP", m);

	g->bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0, 100000);
}