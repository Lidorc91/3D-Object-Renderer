#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

class Camera {
public:
	glm::mat4 _viewMatrix;
	void Recenter();
};

