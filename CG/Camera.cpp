#include "Camera.h"

Camera::Camera() : _viewMatrix(1.0f), _eye(0.0f, 0.0f, 1.0f), _center(0.0f, 0.0f, 0.0f), _up(0.0f, 1.0f, 0.0f) {
	UpdateViewer();
}

void Camera::RepositionCamera(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) {
	//Get new parameters
	_eye = eye;
	_center = center;
	_up = up;
	//update view matrix
	UpdateViewer();
	//Recenter - Use Lookat function
}

void Camera::UpdateViewer() {
	//constrcut the LookAt matrix
	/*
	//view_matrix paramteres
	glm::vec3 forward = glm::normalize(_center - _eye);
	glm::vec3 right = glm::normalize(glm::cross(forward, _up));
	glm::vec3 trueUp = glm::cross(right, forward);

	// Build the rotation matrix
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation[0][0] = right.x;  rotation[1][0] = right.y;  rotation[2][0] = right.z;
	rotation[0][1] = trueUp.x; rotation[1][1] = trueUp.y; rotation[2][1] = trueUp.z;
	rotation[0][2] = -forward.x; rotation[1][2] = -forward.y; rotation[2][2] = -forward.z;

	// Build the translation matrix
	glm::mat4 translation = glm::mat4(1.0f);
	translation[3][0] = -_eye.x;
	translation[3][1] = -_eye.y;
	translation[3][2] = -_eye.z;

	_viewMatrix = rotation * translation;
	*/

	_viewMatrix = glm::lookAt(_eye, _center, _up);
}
