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
float pitch=0, yaw=0;
Geometry* g;
float spd = 0.1;
std::vector<float> data;
std::vector<unsigned int> indices;

void Draw::init() {
	glEnable(GL_DEPTH_TEST);
	glPointSize(4);

	float p = 0.1;
	int pc = 2*3.141592f / p;
	for (float v = 0; v < 3.141592f; v+=p) {
		for (float h = 0; h < 2 * 3.141592f; h+=p) {
			data.push_back(cos(h)*sin(v));//x
			data.push_back(cos(v));//y
			data.push_back(sin(h)*sin(v));//z
		}
	}

	int indexCounter = 0;
	while (indexCounter < data.size() / 3) {
		indices.push_back(indexCounter);
		indices.push_back(indexCounter + 1);
		indices.push_back(indexCounter + pc);

		indices.push_back(indexCounter + pc);
		indices.push_back(indexCounter + 1);
		indices.push_back(indexCounter + pc + 1);
		++indexCounter;
	}

	g = new Geometry();
	g->indexData(indices.size() * sizeof(unsigned int), indices.data());
	g->vertexData(data.size()*sizeof(float), data.data());
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
		"uniform mat4 MVP;\n"
		"void main(){\n"
		"v_pos = inPos;\n"
		"gl_Position = MVP*vec4(inPos, 1.0);\n"
		"}";

	std::string fSrc =
		"layout (location = 0) out vec4 outCol;"
		"in vec3 v_pos;\n"
		"void main(){\n"
		"outCol = vec4(abs(v_pos), 1.0);\n"
		"}";
	
	s = new Shader(vSrc, fSrc);
	s->use();

	c = new Camera();

	Window::setRelativeMouseMode(true);
	Window::keyboardCallback = []() {
		if (window_key(ESCAPE))
			Window::shouldClose = true;

		if (window_key(X)) {
			c->position = glm::vec3(0.f, 0.f, -4.1f);
			yaw = 0;
			pitch = 0;
			c->rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
		}

		if (window_key(Z)) {
			c->position = glm::vec3(0.f, 0.f, -4.1f);
			yaw = 180;
			pitch = 0;
			c->rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
		}

		if (window_key(LALT))
			Window::setRelativeMouseMode(!Window::isRelativeMouseMode());

		if (window_key(LSHIFT))
			spd *= 1.1;

		if (window_key(LCTRL))
			spd /= 1.1;
	};
}

void Draw::update() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (window_key(W))
		c->moveForward(0.3f * spd);

	if (window_key(S))
		c->moveForward(-0.3f * spd);

	if (window_key(D))
		c->moveRight(0.3f * spd);

	if (window_key(A))
		c->moveRight(-0.3f * spd);

	if (window_key(E))
		c->moveUp(0.3f * spd);

	if (window_key(Q))
		c->moveUp(-0.3f * spd);

	if(window_key(R))
		c->rotation *= glm::angleAxis(glm::radians(1.f), c->getForward());

	if (window_key(F))
		c->rotation *= glm::angleAxis(glm::radians(-1.f), c->getForward());

	if (Window::isRelativeMouseMode()) {
		pitch -= Window::mouseDY;
		yaw -= Window::mouseDX;
		//c->rotation *= glm::angleAxis(glm::radians((float)-Window::mouseDY), c->getRight());
		//c->rotation *= glm::angleAxis(glm::radians((float)-Window::mouseDX), c->getUp());
	}
	
	glm::vec3 right;
	right.x = cos(glm::radians(yaw));
	right.y = 0.f;
	right.z = sin(-glm::radians(yaw));
	ImGui::Text("right   (%f,%f,%f)", right.x, right.y, right.z);

	c->rotation = glm::angleAxis(glm::radians(pitch), right) * glm::angleAxis(glm::radians(yaw), c->worldUp);

	glm::mat4 m(1.0f);
	m *= glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 100000.0f);
	m *= c->getLookatMatrix();
	m = glm::translate(m, glm::vec3(0.0f, 0.0f, -5.0f));
	s->uniform("MVP", m);

	g->bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}