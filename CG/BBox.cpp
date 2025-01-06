#include "BBox.h"

	BBox::BBox() : _boxPoints(), _boxEdges(), _max{ 0, 0, 0 }, _min{ 0, 0, 0 }, _center{ 0, 0, 0 } {}
	void BBox::CreateBBox(Wavefront_obj& wf) {
		//Reset BBox
		Reset();
		//Caclulate Min and Max for BBox
		for (auto& point : wf.m_points) {
			UpdateMax(point);
			UpdateMin(point);
		}		
		//calculate center
		this->UpdateCenter();
		UpdateScale();
		CalcDiamiter();
		//Generate points for BBox
		GeneratePoints();
		//Generate edges for BBox
		GenerateEdges();
	}
	
	inline void BBox::UpdateMax(const Wavefront_obj::Vector& point) {
		_max.x = (_max.x < point[0]) ? point[0] : _max.x;
		_max.y = (_max.y < point[1]) ? point[1] : _max.y;
		_max.z = (_max.z < point[2]) ? point[2] : _max.z;
	}
	inline void BBox::UpdateMin(const Wavefront_obj::Vector& point) {
		_min.x = (_min.x > point[0]) ? point[0] : _min.x;
		_min.y = (_min.y > point[1]) ? point[1] : _min.y;
		_min.z = (_min.z > point[2]) ? point[2] : _min.z;
	}
	
	void BBox::UpdateScale() {
		_scale.x = (_max.x - _min.x) != 0 ? 1 / (_max.x - _min.x) : 0;
		_scale.y = (_max.y - _min.y) != 0 ? 1 / (_max.y - _min.y) : 0;
		_scale.z = (_max.z - _min.z) != 0 ? 1 / (_max.z - _min.z) : 0;
	}
	void BBox::GeneratePoints() {
		//Generate points for BBox
		_boxPoints.emplace_back(glm::vec4((_min.x - _center.x) / diamitermax, (_min.y - _center.y) / diamitermax, (_min.z - _center.z) / diamitermax, 1.0f));
		_boxPoints.emplace_back(glm::vec4((_min.x - _center.x) / diamitermax, (_min.y - _center.y) / diamitermax, (_max.z - _center.z) / diamitermax, 1.0f));
		_boxPoints.emplace_back(glm::vec4((_min.x - _center.x) / diamitermax, (_max.y - _center.y) / diamitermax, (_min.z - _center.z) / diamitermax, 1.0f));
		_boxPoints.emplace_back(glm::vec4((_min.x - _center.x) / diamitermax, (_max.y - _center.y) / diamitermax, (_max.z - _center.z) / diamitermax, 1.0f));
		_boxPoints.emplace_back(glm::vec4((_max.x - _center.x) / diamitermax, (_min.y - _center.y) / diamitermax, (_min.z - _center.z) / diamitermax, 1.0f));
		_boxPoints.emplace_back(glm::vec4((_max.x - _center.x) / diamitermax, (_min.y - _center.y) / diamitermax, (_max.z - _center.z) / diamitermax, 1.0f));
		_boxPoints.emplace_back(glm::vec4((_max.x - _center.x) / diamitermax, (_max.y - _center.y) / diamitermax, (_min.z - _center.z) / diamitermax, 1.0f));
		_boxPoints.emplace_back(glm::vec4((_max.x - _center.x) / diamitermax, (_max.y - _center.y) / diamitermax, (_max.z - _center.z) / diamitermax, 1.0f));
	}
	void BBox::GenerateEdges() { //CHECK IF THIS IS CORRECT
		//Generate edges for BBox
		_boxEdges.insert({ 0,1 });
		_boxEdges.insert({ 0,2 });
		_boxEdges.insert({ 0,4 });
		_boxEdges.insert({ 1,3 });
		_boxEdges.insert({ 1,5 });
		_boxEdges.insert({ 2,3 });
		_boxEdges.insert({ 2,6 });
		_boxEdges.insert({ 3,7 });
		_boxEdges.insert({ 4,5 });
		_boxEdges.insert({ 4,6 });
		_boxEdges.insert({ 5,7 });
		_boxEdges.insert({ 6,7 });
	}

	glm::vec3 BBox::getMax() const {
	return _max;
	}

	glm::vec3 BBox::getMin() const {
		return _min;
	}

	glm::vec3 BBox::getCenter() const {
		return _center;
	}


	//Updating center only for wavefront obj
	void BBox::UpdateCenter() {
		//culc center of BB
		_center = (_max + _min) * 0.5f;
		centermax = _center[0];
		if (_center[1] > centermax) { centermax = _center[1]; }
		if (_center[2] > centermax) { centermax = _center[2]; }
	}
	void BBox::CalcDiamiter() {


		_BB_diamiter[0] = _max[0] - _min[0];
		_BB_diamiter[1] = _max[1] - _min[1];
		_BB_diamiter[2] = _max[2] - _min[2];

		diamitermax = _BB_diamiter[0];
		if (_BB_diamiter[1] > diamitermax) { diamitermax = _BB_diamiter[1]; }
		if (_BB_diamiter[2] > diamitermax) { diamitermax = _BB_diamiter[2]; }
	}
	//Reset BBox
	void BBox::Reset() {
		_boxPoints.clear();
		_boxEdges.clear();
		_max = { -INFINITY , -INFINITY, -INFINITY };
		_min = { INFINITY , INFINITY, INFINITY };
		_center = { 0, 0, 0 };
	}

