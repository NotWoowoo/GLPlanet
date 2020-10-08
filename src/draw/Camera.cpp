#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Camera.h"

#include <imgui.h>

Camera::Camera(glm::vec3 position, glm::quat rotation)
	: position(position), rotation(rotation) {
	calculateOrientationVectors();
}

void Camera::moveForward(float amt){
	position += amt * forward;
}

void Camera::moveUp(float amt){
	position += amt * up;
}

void Camera::moveRight(float amt){
	position += amt * right;
}

void Camera::moveTowards(float amt, glm::vec3 target){
	if(target != position)
		position += amt * glm::normalize(target - position);
}

void Camera::applyRotation(glm::quat q){
	glm::quat temp = rotation;
	rotation = q;
	rotation *= temp;
	calculateOrientationVectors();
}

void Camera::unapplyRotation(glm::quat q) {
	glm::quat temp = rotation;
	rotation = glm::inverse(q);
	rotation *= temp;
	calculateOrientationVectors();
}


const glm::vec3& Camera::getForward() { return forward; }

const glm::vec3& Camera::getRight() { return right; }

const glm::vec3& Camera::getUp() { return up; }

void Camera::calculateOrientationVectors() {
	right = glm::rotate(rotation, worldRight);
	up = glm::rotate(rotation, worldUp);
	forward = glm::rotate(rotation, worldForward);
}

const glm::mat4& Camera::getLookatMatrix() {
	ImGui::Text("pos    (%f,%f,%f)", position.x, position.y, position.z);
	float rotAng = 2 * acos(rotation.w);
	float div = sin(rotAng / 2.f);
	ImGui::Text("rot %f (%f,%f,%f)", glm::degrees(rotAng), rotation.x / div, rotation.y / div, rotation.z / div);

	calculateOrientationVectors();

	//ImGui::Text("forward (%f,%f,%f)", forward.x, forward.y, forward.z);
	//ImGui::Text("right   (%f,%f,%f)", right.x, right.y, right.z);
	//ImGui::Text("up      (%f,%f,%f)", up.x, up.y, up.z);

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

void Camera::lookAt(glm::vec3 target) {
	//rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
	glm::vec3 newDirection = glm::normalize(target - position);

	if (glm::dot(newDirection, forward) > 0.999999f)
		return;

	if (glm::dot(newDirection, forward) < -0.999999f) {
		applyRotation(glm::angleAxis(glm::radians(180.f), up));
		return;
	}

	glm::vec3 rotationAxis = glm::normalize(glm::cross(newDirection, forward));
	float rotationAngle = acos(glm::dot(newDirection, forward));

	applyRotation(glm::angleAxis(-rotationAngle, rotationAxis));

	/*printf("newDirection (%f, %f, %f)\n\n", newDirection.x, newDirection.y, newDirection.z);
	printf("rotationAxis (%f, %f, %f)\n", rotationAxis.x, rotationAxis.y, rotationAxis.z);
	printf("angle        %f\n", glm::degrees(rotationAngle));
	printf("worked? (%f, %f, %f)\n\n", forward.x, forward.y, forward.z);
	printf("\n\n----------\n\n");*/
}