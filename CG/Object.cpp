#include "Object.h"
#include <iostream>


	//Object creation
	Object::Object(Wavefront_obj& wf) : _modelMatrix(1.0f), _translationMatrix(1.0f), _rotationMatrix(1.0f), _scaleMatrix(1.0f) {
		//Intialize meshmodel
		_meshModel = MeshModel();
		//Create Bbox and calculate center
		_box = BBox(wf);
		TransferToWorldFrame(wf);
		//Isotropic scale by 10
		Scale(10);
		Transform();
		_scaleMatrix = (1.0f);
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
	}
	void Object::Translate(float x, float y, float z) {
		_translationMatrix[3] = glm::vec4(x, y, z, 1.0f);
	}
	void Object::Rotate(float x, float y, float z) {
		glm::mat4 rotateX(float x);
		glm::mat4 rotateY(float y);
		glm::mat4 rotateZ(float z);
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
		for each (const Wavefront_obj::Vector& point in wf.m_points)
		{			
			glm::vec4 newVec;
			//recenter point + normalize
			newVec.x = point[0] - _box.getCenter().x;
			newVec.x /= abs(_box.getMax().x - _box.getCenter().x);

			newVec.y = point[1] - _box.getCenter().y;
			newVec.y /= abs(_box.getMax().y - _box.getCenter().y);

			newVec.z = point[2] - _box.getCenter().z;
			newVec.z /= abs(_box.getMax().z - _box.getCenter().z);

			//create homogenous coordinate
			newVec.w = 1;
			//add point to Meshmodel
			_meshModel._points.push_back(newVec);
		}
		for each (const Wavefront_obj::Face& face in wf.m_faces)
		{
			glm::vec3 newFace(face.v[0], face.v[1], face.v[2]);  
			_meshModel._faces.push_back(newFace);
		}
	}

	void Object::ResetMatrices() {
		_translationMatrix = (1.0f);
		_rotationMatrix = (1.0f);
		_scaleMatrix = (1.0f);		
	}
