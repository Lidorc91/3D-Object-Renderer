#pragma once

#include <glm/vec3.hpp>
#include "MeshModel.h"
#include "Obj Parser/wavefront_obj.h"

class BBox
{
public:
	glm::vec3 _max = { -INFINITY , -INFINITY, -INFINITY };
	glm::vec3 _min = { INFINITY , INFINITY, INFINITY };
	glm::vec3 _center;

	std::vector<glm::vec4> _points;
	std::set<pair<int, int>> _edges;

	BBox();
	void CreateBBox(Wavefront_obj& wf);
	void Reset();

	void UpdateCenter(MeshModel& model);	
	inline void UpdateMax(const Wavefront_obj::Vector& point);
	inline void UpdateMin(const Wavefront_obj::Vector& point);
	glm::vec3 getCenter() const;
	glm::vec3 getMax() const;
	glm::vec3 getMin() const;
private:
	void UpdateCenter();
	void GeneratePoints();
	void GenerateEdges();
};
