#include "BBox.h"

	BBox::BBox() : _max{ 0, 0, 0 }, _min{ 0, 0, 0 }, _center{ 0, 0, 0 } {}
	BBox::BBox(Wavefront_obj& wf) : _max{ 0, 0, 0 }, _min{ 0, 0, 0 }, _center{ 0, 0, 0 } {
		//Recenter points and add to meshmodel
		for (auto& point : wf.m_points) {
			UpdateMax(point);
			UpdateMin(point);
		}
		//calculate center
		this->UpdateCenter();
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

