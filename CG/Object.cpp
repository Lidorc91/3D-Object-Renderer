#include "Object.h"
#include "MeshModel.h"
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

class Object {
public:
	static glm::mat4 Model; //model matrix -> brings all objects to to world space
	MeshModel m; //Geometry data

	//Add OBJECT transoformations

};