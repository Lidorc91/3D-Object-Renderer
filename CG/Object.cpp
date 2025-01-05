#include "Object.h"
#include <iostream>

	
	//Object creation
	Object::Object() : _modelMatrix(1.0f), _translationMatrix(1.0f), _rotationMatrix(1.0f), _scaleMatrix(1.0f) {
		_meshModel = MeshModel();
		_box = BBox();
	}

	void Object::ReadFile(Wavefront_obj& wf){
		//Intialize meshmodel
		_meshModel.CreateMeshModel(wf);
		//Create Bbox and calculate center
		_box.CreateBBox(wf);
		//Bring obj coordinates to World frame (recenter) + Normalize coordinates + add obj coordinates + homogenous coordinates to meshmodel
		RecenterAndNormalize(wf);
		//Isotropic scale by 10
		Scale(5);
		//Rotate 90 degrees around X-axis
		//Rotate(90, 0, 45); // Change to combine rotations
		//Translate by 1 in X-axis
		//Translate(1, 0, 0);
		std::cout << "Object Created" << std::endl;
	}

	//Add OBJECT transformations
	void Object::Transform() {
		//Trigger relevant transform function

		//final calculation
		_modelMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;		
	}

	void Object::Scale(float s) {
		_scaleMatrix = glm::mat4(s, 0, 0, 0,
			0, s, 0, 0,
			0, 0, s, 0,
			0, 0, 0, 1);
		//Transform();
		//Update BBox
	}
	void Object::Translate(float x, float y, float z) {
		_translationMatrix[3] = glm::vec4(x, y, z, 1.0f);

		//Transform();
		//Update BBox
	}

	void Object::Rotate(float x, float y, float z) { // TODO - Change to switch case (can only pick 1 axis at a time w/ amount of rotation)
		glm::mat4 Rx = x != 0 ? rotateX(x) : glm::mat4(1.0f);
		glm::mat4 Ry = y != 0 ? rotateY(y) : glm::mat4(1.0f);
		glm::mat4 Rz = z != 0 ? rotateZ(z) : glm::mat4(1.0f);

		_rotationMatrix = Rx * Ry * Rz;
		//Transform();
		//Update BBox
	}

	// Create rotation matrix around X-axis
	glm::mat4 Object::rotateX(float angle) {
		double rad = angle * M_PI / 180.0;
		glm::mat4 Rx = glm::mat4(1.0f);
		Rx[1][1] = cos(rad);
		Rx[1][2] = -sin(rad);
		Rx[2][1] = sin(rad);
		Rx[2][2] = cos(rad);	

		return Rx;
	}

	// Create rotation matrix around Y-axis
	glm::mat4 Object::rotateY(float angle) {
		double rad = angle * M_PI / 180.0;
		glm::mat4 Ry = glm::mat4(1.0f);
		Ry[0][0] = cos(rad);
		Ry[0][2] = sin(rad);
		Ry[2][0] = -sin(rad);
		Ry[2][2] = cos(rad);

		return Ry;
	}

	// Create rotation matrix around Z-axis
	glm::mat4 Object::rotateZ(float angle) {
		double rad = angle * M_PI / 180.0;
		glm::mat4 Rz = glm::mat4(1.0f);
		Rz[0][0] = cos(rad);
		Rz[0][1] = -sin(rad);
		Rz[1][0] = sin(rad);
		Rz[1][1] = cos(rad);

		return Rz;
	}

	//Bring obj coordinates to World frame (recenter) + Normalize coordinates + add obj coordinates + homogenous coordinates to meshmodel
	void Object::RecenterAndNormalize(Wavefront_obj& wf) {
		//recenter
		glm::vec4 center = { _box._center.x,_box._center.y,_box._center.z,0};
		for (auto& vertex : _meshModel._points) {
			vertex -= center;
		}
		//normalize
		glm::vec3 halfRange = (_box._max - _box._min) * 0.5f;
		float maxHalfRange = std::max({ halfRange.x, halfRange.y, halfRange.z });
		if (maxHalfRange > 0.0f) {
			for (auto& vertex : _meshModel._points) {
				vertex /= maxHalfRange;
				vertex.w = 1.0f;
			}
		}
	}

	void Object::ResetMatrices() {
		_translationMatrix = (1.0f);
		_rotationMatrix = (1.0f);
		_scaleMatrix = (1.0f);		
	}
