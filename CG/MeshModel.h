#pragma once

#include <vector>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <set>
#include "Obj Parser/wavefront_obj.h"
#include <array>
using namespace std;

class MeshModel
{
public:
	//Vectors to store positions
	std::vector<glm::vec4> _points;
	std::vector<pair<std::array<int,3>, glm::vec4>> _normals;
	std::set<pair<int,int>> _edges;
	MeshModel();
	MeshModel(Wavefront_obj& wf);
	
};

