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
	void UpdatePerspective(float near, float far, float fov, float aspect);
	void UpdateAspectRatio(float aspect) { _aspectRatio = aspect; UpdatePerspective(_near, _far, _fov, _aspectRatio); } //Update on Window Resize
	glm::mat4 _viewMatrix; //position in world
	glm::mat4 _projectionMatrix; //Distort (perspective->orthogonal) and Project 
	float _aspectRatio;  // Screen aspect ratio
private:
	//View Matrix
	glm::vec3 _eye, _target, _up;	

	//Perspective Matrix (NDC)
	float _near = 0.1f;  // Near clipping plane
	float _far = 1000.0f;  // Far clipping plane
	float _fov = 45.0f;  // Field of View


	//glm::mat4 _projectionMatrix = glm::perspective(fov, aspect, near, far);
	
};

