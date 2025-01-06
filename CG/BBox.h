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
	glm::vec3 _scale;
	glm::vec3 _BB_diamiter;

	float diamitermax;
	float centermax;

	std::vector<glm::vec4> _boxPoints;
	std::set<pair<int, int>> _boxEdges;

	BBox();
	void CreateBBox(Wavefront_obj& wf);
	void Reset();

	void UpdateCenter(MeshModel& model);	
	inline void UpdateMax(const Wavefront_obj::Vector& point);
	inline void UpdateMin(const Wavefront_obj::Vector& point);
	void UpdateScale();
	void CalcDiamiter();
	glm::vec3 getCenter() const;
	glm::vec3 getMax() const;
	glm::vec3 getMin() const;
private:
	void UpdateCenter();
	void GeneratePoints();
	void GenerateAxis();
	void GenerateEdges();
};
