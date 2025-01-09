#pragma once

#include <vector>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <set>
#include "Obj Parser/wavefront_obj.h"
#include <array>
#include <unordered_map>
using namespace std;

class MeshModel
{
public:
	//Vectors to store positions
	std::vector<glm::vec4> _points;
	std::vector<pair<std::array<int,3>, glm::vec4>> _faceNormals;
	std::unordered_map<int, std::vector<int>> _pointNormalsMap;
	std::unordered_map<int, glm::vec4> _pointNormals;
	std::set<pair<int,int>> _edges;
	MeshModel();
	void CreateMeshModel(Wavefront_obj& wf);
	void Reset();
	
};

