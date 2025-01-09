#pragma once

#include "MeshModel.h"
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <string>
#include "BBox.h"
#include "Obj Parser/wavefront_obj.h"
#define _USE_MATH_DEFINES
#include <math.h>



enum TransformType
{
	ObjectTransform,
	WorldTransform
};

class Object
{
public:
	std::string _name;
	glm::mat4 _objectModelMatrix; //model matrix -> brings all objects to to world space
	glm::mat4 _worldModelMatrix; //Transforms in world space

	MeshModel _meshModel; //Geometry data
	BBox _box;
	//Transform matrices - to calculate final translation
	glm::mat4 _objectTranslationMatrix;
	glm::mat4 _worldTranslationMatrix;
	glm::mat4 _objectRotationMatrix;
	glm::mat4 _scaleMatrix;

	//Axis matrices - to scale/rotate axis & normals
	glm::mat4 _worldRotationMatrix = glm::mat4(1.0f);
	glm::mat4 _worldScaleMatrix = glm::mat4(1.0f);

	std::vector<glm::vec4> _ObjectAxisPoints;
	std::set<pair<int, int>> _ObjectAxisEdges;
	std::vector<glm::vec4> _WorldAxisPoints;
	std::set<pair<int, int>> _WorldAxisEdges;

	float _scale = 1.0f;
	float _translateX = 0.0f;
	float _translateY = 0.0f;
	float _translateZ = 0.0f;
	float _rotateX = 0.0f;
	float _rotateY = 0.0f;
	float _rotateZ = 0.0f;

	

	Object();
	void ReadFile(Wavefront_obj& wf);
	void RecenterAndNormalize(Wavefront_obj& wf);
	void Transform();
	void ResetMatrices();
	void GenerateObjectAxis();
	void GenerateWorldAxis();
	void Scale(float s, TransformType t);
	void Translate(float x, float y, float z, TransformType t);
	void Rotate(float x, float y, float z, TransformType t);
	glm::mat4 rotateX(float angle);
	glm::mat4 rotateY(float angle);
	glm::mat4 rotateZ(float angle);
};


