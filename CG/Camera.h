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

private:
	//View Matrix
	glm::vec3 _eye, _center, _up;
	glm::mat4 _viewMatrix;

	//Perspective Matrix (NDC)
	float fov;        // Field of view in radians
	float aspect;     // Aspect ratio 
	float near;       // Near clipping plane
	float far;		  // Far clipping plane

	glm::mat4 projectionMatrix = glm::perspective(fov, aspect, near, far);

	//Screen coordinates
	viewportTransform(glm::vec4 ndc, float width, float height)
};

