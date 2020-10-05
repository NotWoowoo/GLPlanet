#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class Camera
{
public:
	glm::vec3 position;
	glm::quat rotation;
	const glm::vec3
		worldForward = glm::vec3(0.f, 0.f, -1.f),
		worldRight = glm::vec3(1.f, 0.f, 0.f),
		worldUp = glm::vec3(0.f, 1.f, 0.f);

	Camera(glm::vec3 position = glm::vec3(0,0,0), glm::quat rotation = glm::quat(1.f,0.f,0.f,0.f));

	void moveForward(float amt);
	void moveUp(float amt);
	void moveRight(float amt);

	const glm::vec3& getForward();
	const glm::vec3& getRight();
	const glm::vec3& getUp();

	void calculateOrientationVectors();

	const glm::mat4& getLookatMatrix();

	void lookAt(glm::vec3 position);

private:
	glm::vec3 forward, right, up;
	glm::mat4 lookatMatrix = glm::mat4(1.0f);
};

