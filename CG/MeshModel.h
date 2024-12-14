#pragma once

#include <vector>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class MeshModel
{
public:
	//Vectors to store positions
	std::vector<glm::vec4> _points;
	std::vector<glm::vec3> _faces;
	MeshModel();
};

