#pragma once

#include "MeshModel.h"
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <string>
#include "BBox.h"
#include "Obj Parser/wavefront_obj.h"
#define _USE_MATH_DEFINES
#include <math.h>

class Object
{
public:
	std::string _name;
	glm::mat4 _modelMatrix; //model matrix -> brings all objects to to world space
	MeshModel _meshModel; //Geometry data
	BBox _box;
	glm::mat4 _translationMatrix;
	glm::mat4 _rotationMatrix;
	glm::mat4 _scaleMatrix;

	Object(Wavefront_obj& wf);
	void TransferToWorldFrame(Wavefront_obj& wf);
	void Transform();
	void ResetMatrices();
private:
	void Scale(float s);
	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);
};

