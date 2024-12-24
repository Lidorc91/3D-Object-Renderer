#include "Camera.h"

//Lookat -> Projection -> Viewport
Camera::Camera() : _viewMatrix(1.0f), _eye(0.0f, 0.0f, 15.0f), _target(0.0f, 0.0f, 0.0f), _up(0.0f, 1.0f, 0.0f) {
	UpdateViewer();
	//UpdatePerspective(_near, _far, _fov, _aspectRatio);	
}

void Camera::RepositionCamera(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) {
	//Get new parameters
	_eye = eye;
	_target = target;
	_up = up;
	//Recenter - Use Lookat function
	UpdateViewer();
}

void Camera::UpdateViewer() {
	//constrcut the LookAt matrix
	
	//view_matrix paramteres
	glm::vec3 forward = glm::normalize(_target - _eye);
	glm::vec3 right = glm::normalize(glm::cross(forward, _up));
	glm::vec3 trueUp = glm::cross(right, forward);

	// Build the translation matrix
	glm::mat4 translation = glm::mat4(1.0f);
	translation[3][0] = -_eye.x;
	translation[3][1] = -_eye.y;
	translation[3][2] = -_eye.z;

	// Build the rotation matrix
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation[0][0] = right.x;  rotation[1][0] = right.y;  rotation[2][0] = right.z;
	rotation[0][1] = trueUp.x; rotation[1][1] = trueUp.y; rotation[2][1] = trueUp.z;
	rotation[0][2] = -forward.x; rotation[1][2] = -forward.y; rotation[2][2] = -forward.z;

	_viewMatrix = rotation * translation;	

	//_viewMatrix = glm::lookAt(_eye, _target, _up);
}

void Camera::UpdatePerspective(float near, float far, float right, float top){

	_near = near;
	_far = far;
	_right = right;
	_top = top;

	_projectionMatrix = glm::mat4x4(_near / _right, 0.0f, 0.0f, 0.0f,//first column
		0.0f, _near / _top, 0.0f, 0.0f,//second column
		0.0f, 0.0f, -float((_far + _near)) / float((_far - _near)), (-2 * float(_far * _near)) / float((_far - _near)),//third column
		0.0f, 0.0f,-1.0f , 0.0f);
}

void Camera::UpdatePerspective(float near, float far, float fov) {
	_near = near;
	_far = far;
	_fov = fov;

	//extract right and top
	_top = _near * tan(glm::radians(_fov) / 2);  // Top of the frustum
	_right = _top * _aspectRatio;        // Right of the frustum

	//update perspective matrix
	_projectionMatrix = glm::mat4x4(_near / _right, 0.0f, 0.0f, 0.0f,//first column
		0.0f, _near / _top, 0.0f, 0.0f,//second column
		0.0f, 0.0f, -float((_far + _near)) / float((_far - _near)), (-2 * float(_far * _near)) / float((_far - _near)),//third column
		0.0f, 0.0f, -1.0f, 0.0f);
}