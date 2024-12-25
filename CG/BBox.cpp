#include "BBox.h"

	BBox::BBox() : _boxPoints(), _boxEdges(), _max{ 0, 0, 0 }, _min{ 0, 0, 0 }, _center{ 0, 0, 0 } {}
	BBox::BBox(Wavefront_obj& wf) : _max{ 0, 0, 0 }, _min{ 0, 0, 0 }, _center{ 0, 0, 0 } {
		//Recenter points and add to meshmodel
		for (auto& point : wf.m_points) {
			UpdateMax(point);
			UpdateMin(point);
		}
		//calculate center
		this->UpdateCenter();
		//Generate points for BBox
		GeneratePoints();
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
	void BBox::GeneratePoints() {
		//Generate points for BBox
		_boxPoints.push_back({ _max.x,_max.y,_max.z ,1});
		_boxPoints.push_back({ _max.x,_max.y,_min.z ,1});
		_boxPoints.push_back({ _max.x,_min.y,_max.z ,1});
		_boxPoints.push_back({ _max.x,_min.y,_min.z ,1});
		_boxPoints.push_back({ _min.x,_max.y,_max.z ,1});
		_boxPoints.push_back({ _min.x,_max.y,_min.z ,1});
		_boxPoints.push_back({ _min.x,_min.y,_max.z ,1});
		_boxPoints.push_back({ _min.x,_min.y,_min.z ,1});
		
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
		_center.x = (_max.x + _min.x) / 2;
		_center.y = (_max.y + _min.y) / 2;
		_center.z = (_max.z + _min.z) / 2;
	}

