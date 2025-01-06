#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	Camera();
	void RepositionCamera(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
	void UpdateViewer();
	void UpdatePerspective(float near, float far, float right, float top);
	void UpdatePerspective(float near, float far, float fov);
	void UpdateAspectRatio(float aspect) { _aspectRatio = aspect; UpdatePerspective(_near, _far, _fov); } //Update on Window Resize
	void ResetViewMatrix();
	void ResetMatrices() { _viewMatrix = glm::mat4(1.0f); _projectionMatrix = glm::mat4(1.0f); }
	glm::mat4 _viewMatrix; //position in world
	glm::mat4 _projectionMatrix; //Distort (perspective->orthogonal) and Project 
	float _aspectRatio;  // Screen aspect ratio
	//View Matrix
	glm::vec3 _eye, _target, _up;

	//Perspective Matrix (NDC)
	float _near = 1.0f;  // Near clipping plane
	float _far = 1000.0f;  // Far clipping plane
	float _right = 1.0f;  // Right of the frustum
	float _top = 1.0f;  // Top of the frustum
	float _fov = 60.0f;  // Field of View
private:


	//glm::mat4 _projectionMatrix = glm::perspective(fov, aspect, near, far);
	
};

