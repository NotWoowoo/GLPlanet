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

struct {
	Camera* c;
	float pitch = 0, yaw = 0;
	glm::vec2 planetPos = glm::vec2(0.f);
	float planetHeading = 0.f;
	float height = 1.2f;

	void moveForward(float amt) {
		planetPos.x += amt * sin(planetHeading);
		planetPos.y += amt * cos(planetHeading);
	}

	void moveRight(float amt) {
		planetPos.x += amt * sin(planetHeading + glm::radians(90.f));
		planetPos.y += amt * cos(planetHeading + glm::radians(90.f));
	}

} player;

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
		"outCol = vec4(abs(sin(v_pos*100)), 1.0);\n"
		"}";
	
	s = new Shader(vSrc, fSrc);
	s->use();

	player.c = new Camera();

	Window::setRelativeMouseMode(true);
	Window::keyboardCallback = []() {
		if (window_key(ESCAPE))
			Window::shouldClose = true;

		/*if (window_key(X)) {
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

		if (window_key(O))
			c->lookAt(glm::vec3(0.0f, 0.0f, -5.0f));*/

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
		player.moveForward(0.03f * spd);

	if (window_key(S))
		player.moveForward(-0.03f * spd);

	if (window_key(D))
		player.moveRight(0.03f * spd);

	if (window_key(A))
		player.moveRight(-0.03f * spd);

	/*if (window_key(E))
		c->moveUp(0.3f * spd);

	if (window_key(Q))
		c->moveUp(-0.3f * spd);

	if (window_key(R)) {
		c->applyRotation(glm::angleAxis(glm::radians(1.f), c->getForward()));
	}

	if (window_key(F))
		c->applyRotation(glm::angleAxis(glm::radians(-1.f), c->getForward()));

	if (window_key(V))
		c->applyRotation(glm::angleAxis(glm::radians(1.f), c->getUp()));

	if (window_key(B))
		c->applyRotation(glm::angleAxis(glm::radians(1.f), c->getRight()));*/

	if (Window::isRelativeMouseMode()) {
		player.pitch -= Window::mouseDY;
		player.yaw -= Window::mouseDX;
		//c->rotation *= glm::angleAxis(glm::radians((float)-Window::mouseDY), c->getRight());
		//c->rotation *= glm::angleAxis(glm::radians((float)-Window::mouseDX), c->getUp());
	}

	player.c->position = glm::vec3(
		player.height * sin(player.planetPos.x),
		player.height * sin(player.planetPos.y)*cos(player.planetPos.x),
		player.height * cos(player.planetPos.x)*cos(player.planetPos.y)
	);

	printf("%f\n", glm::length(player.c->position));
	
	/*glm::vec3 right;
	right.x = cos(glm::radians(yaw));
	right.y = 0.f;
	right.z = sin(-glm::radians(yaw));*/

	//if(glm::distance(c->position, glm::vec3(0.0f, 0.0f, -5.0f)) > 1.2f)
		//c->moveTowards(0.1f, glm::vec3(0.0f, 0.0f, -5.0f));
	
	//c->rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
	//c->unapplyRotation(glm::angleAxis(glm::radians(pitch), right) * glm::angleAxis(glm::radians(yaw), c->worldUp));
	//player.c->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	//c->applyRotation(glm::angleAxis(glm::radians(pitch), right) * glm::angleAxis(glm::radians(yaw), c->worldUp));

	glm::mat4 m(1.0f);
	m *= glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 100000.0f);
	m *= player.c->getLookatMatrix();
	//m = glm::translate(m, glm::vec3(0.0f, 0.0f, -5.0f));
	s->uniform("MVP", m);

	g->bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}