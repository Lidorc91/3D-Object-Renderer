#pragma once

#include <glm/vec3.hpp>
#include "MeshModel.h"
#include "Obj Parser/wavefront_obj.h"

class BBox
{
public:
	glm::vec3 _max;
	glm::vec3 _min;
	glm::vec3 _center;

	std::vector<glm::vec4> _boxPoints;
	std::set<pair<int, int>> _boxEdges;

	BBox();
	BBox::BBox(Wavefront_obj& wf);
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
