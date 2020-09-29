#pragma once

#include <glm\glm.hpp>

class Camera
{
public:
	Camera(glm::vec3 initialPosition, glm::vec3 initialOrientation);
	Camera();

	void setPosition(float x, float y, float z);
	void setOrientation(float pitch, float yaw, float roll);
	glm::vec3 getPosition();
	glm::vec3 getOrientation();

	void addPosition(float x, float y, float z);
	void addOrientation(float pitch, float yaw, float roll);

	void setPitch(float pitch);
	void setYaw(float yaw);
	void setRoll(float roll);
	float getPitch();
	float getYaw();
	float getRoll();

	void setX(float x);
	void setY(float y);
	void setZ(float z);
	float getX();
	float getY();
	float getZ();

	void moveForward(float amount);
	void moveRight(float amount);
	void moveUp(float amount);
	void pan(float h, float v);

	const glm::mat4& getLookatMatrix();

	void lookAt(glm::vec3 position);

private:
	glm::vec3 position, orientation;
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 forward, up, right;
	glm::mat4 lookatMatrix = glm::mat4(1.0f);

	void calculateOrientation();
};

