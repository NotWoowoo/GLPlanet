#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Camera.h"

#include <imgui.h>
#include <string>
#include <glad\glad.h>

#define PI 3.141592
#define TAU PI*2.0f

Camera::Camera(glm::vec3 initialPosition, glm::vec3 initialOrientation)
	: position(initialPosition), orientation(initialOrientation) {
	calculateOrientation(); 
}

Camera::Camera()
	: position(glm::vec3(0.0f, 0.0f, 0.0f)), orientation(glm::vec3(glm::radians(180.0f), 0.0f, 0.0f)) {
	calculateOrientation();
}

void Camera::setPosition(float x, float y, float z)				{ position = glm::vec3(x, y, z);		     }
void Camera::setOrientation(float pitch, float yaw, float roll) { orientation = glm::vec3(pitch, yaw, roll); calculateOrientation(); }
glm::vec3 Camera::getPosition()									{ return position;							 }
glm::vec3 Camera::getOrientation()								{ return orientation;						 }
void Camera::addPosition(float x, float y, float z)				{ position += glm::vec3(x, y, z);			 }
void Camera::addOrientation(float pitch, float yaw, float roll) { orientation += glm::vec3(pitch , yaw, roll); calculateOrientation(); }

void Camera::setPitch(float pitch)  { orientation.x = pitch; calculateOrientation(); }
void Camera::setYaw(float yaw)		{ orientation.y = yaw;  calculateOrientation();  }
void Camera::setRoll(float roll)	{ orientation.z = roll; calculateOrientation();  }
float Camera::getPitch()			{ return orientation.x; }
float Camera::getYaw()				{ return orientation.y; }
float Camera::getRoll()				{ return orientation.z; }

void Camera::setX(float x)			{ position.x = x; }
void Camera::setY(float y)			{ position.y = y; }
void Camera::setZ(float z)			{ position.z = z; }
float Camera::getX()				{ return position.x; }
float Camera::getY()				{ return position.y; }
float Camera::getZ()				{ return position.z; }

void Camera::moveForward(float amount)  { position += amount * forward; }
void Camera::moveRight(float amount)    { position += amount * right; }
void Camera::moveUp(float amount)		{ position += amount * up; }
void Camera::pan(float h, float v) {
	addOrientation(
		h * cos(orientation.z) - v * sin(orientation.z),
		v * cos(orientation.z) + h * sin(orientation.z),
		0);
}

void Camera::calculateOrientation() {
	orientation.x = fmod(orientation.x, TAU);
	orientation.y = fmin(PI/2.f, fmax(-PI/2.f, orientation.y));
	orientation.z = fmod(orientation.z, TAU);
	
	forward.x = sin(-orientation.x) * cos(orientation.y);
	forward.y = sin(orientation.y);
	forward.z = cos(orientation.x) * cos(orientation.y);

	right = glm::normalize(glm::cross(forward, worldUp));
	right = glm::rotate(right, -orientation.z, forward);

	up = glm::cross(right, forward);
}

const glm::mat4& Camera::getLookatMatrix() {
	/*ImGui::Text("magf     %f", glm::length(forward));
	ImGui::Text("magr     %f", glm::length(right));
	ImGui::Text("----------------------------");
	ImGui::Text("dot      %f", glm::dot(right, forward));
	ImGui::Text("pos      (%f, %f, %f)", position.x, position.y, position.z);
	ImGui::Text("----------------------------");
	ImGui::Text("forward  (%f, %f, %f)", forward.x, forward.y, forward.z);
	ImGui::Text("right    (%f, %f, %f)", right.x, right.y, right.z);
	ImGui::Text("----------------------------");
	ImGui::Text("pitch    %f", glm::degrees(orientation.x));
	ImGui::Text("yaw      %f", glm::degrees(orientation.y));
	ImGui::Text("roll     %f", glm::degrees(orientation.z));*/

	lookatMatrix[0][0] = right.x;
	lookatMatrix[1][0] = right.y;
	lookatMatrix[2][0] = right.z;
	lookatMatrix[0][1] = up.x;
	lookatMatrix[1][1] = up.y;
	lookatMatrix[2][1] = up.z;
	lookatMatrix[0][2] = -forward.x;
	lookatMatrix[1][2] = -forward.y;
	lookatMatrix[2][2] = -forward.z;
	lookatMatrix[3][0] = -dot(right, position);
	lookatMatrix[3][1] = -dot(up, position);
	lookatMatrix[3][2] = dot(forward, position);
	
	return lookatMatrix;
}

void Camera::lookAt(glm::vec3 position) {
	orientation = glm::normalize(this->position - position);
}