#include "Object.h"
#include <iostream>


	//Object creation
	Object::Object(Wavefront_obj& wf) : _modelMatrix(1.0f), _translationMatrix(1.0f), _rotationMatrix(1.0f), _scaleMatrix(1.0f) {
		//Intialize meshmodel
		_meshModel = MeshModel(wf);
		//Create Bbox and calculate center
		_box = BBox(wf);
		TransferToWorldFrame(wf);
		//Isotropic scale by 10
		Scale(10);
		_modelMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;
		std::cout << "Object Created" << std::endl;
	}

	//Add OBJECT transformations
	void Object::Transform() {
		//Trigger relevant transform function

		//final calculation
		_modelMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;
		//Reset Matrix operations
		ResetMatrices();
	}

	void Object::Scale(float s) {
		_scaleMatrix = glm::mat4(s, 0, 0, 0,
			0, s, 0, 0,
			0, 0, s, 0,
			0, 0, 0, 1);

		//Update BBox
	}
	void Object::Translate(float x, float y, float z) {
		_translationMatrix[3] = glm::vec4(x, y, z, 1.0f);

		//Update BBox
	}

	void Object::Rotate(float x, float y, float z) { // TODO - Change to switch case (can only pick 1 axis at a time w/ amount of rotation)
		glm::mat4 rotateX(float x);
		glm::mat4 rotateY(float y);
		glm::mat4 rotateZ(float z);

		//Update BBox
	}

	// Create rotation matrix around X-axis
	void Object::rotateX(float angle) {
		double rad = angle * M_PI / 180.0;
		_rotationMatrix[1][1] = cos(rad);
		_rotationMatrix[1][2] = -sin(rad);
		_rotationMatrix[2][1] = sin(rad);
		_rotationMatrix[2][2] = cos(rad);		
	}

	// Create rotation matrix around Y-axis
	void Object::rotateY(float angle) {
		double rad = angle * M_PI / 180.0;
		_rotationMatrix[0][0] = cos(rad);
		_rotationMatrix[0][2] = sin(rad);
		_rotationMatrix[2][0] = -sin(rad);
		_rotationMatrix[2][2] = cos(rad);
	}

	// Create rotation matrix around Z-axis
	void Object::rotateZ(float angle) {
		double rad = angle * M_PI / 180.0;
		_rotationMatrix[0][0] = cos(rad);
		_rotationMatrix[0][1] = -sin(rad);
		_rotationMatrix[1][0] = sin(rad);
		_rotationMatrix[1][1] = cos(rad);
	}

	//Bring obj coordinates to World frame (recenter) + Normalize coordinates + add obj coordinates + homogenous coordinates to meshmodel
	void Object::TransferToWorldFrame(Wavefront_obj& wf) {
		for (glm::vec4& point : _meshModel._points)
		{		
			//recenter point
			point.x -= _box.getCenter().x;
			point.y -= _box.getCenter().y;
			point.z -= _box.getCenter().z;
			
			//normalize
			point.x = (abs(_box.getMax().x - _box.getCenter().x) == 0) ? point.x : point.x / abs(_box.getMax().x - _box.getCenter().x);
			point.y = (abs(_box.getMax().y - _box.getCenter().y) == 0) ? point.y : point.y / abs(_box.getMax().y - _box.getCenter().y);
			point.z = (abs(_box.getMax().z - _box.getCenter().z) == 0) ? point.z : point.z / abs(_box.getMax().z - _box.getCenter().z);
		}

	}

	void Object::ResetMatrices() {
		_translationMatrix = (1.0f);
		_rotationMatrix = (1.0f);
		_scaleMatrix = (1.0f);		
	}
