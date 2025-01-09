#include "Scene.h"

//Add WORLD transoformations

//Constructor
Scene::Scene() : _object() {
	_hasObject = false;
}

// Add an object to the scene
void Scene::setObject(Wavefront_obj& obj) {
	_object.ReadFile(obj);
	_hasObject = true;
}

//Generate ALL transformations (before viewport transformations)
glm::mat4 Scene::GenerateScene() {
	//Update BBox Center using updated translations - TODO
	glm::vec4 center = { _object._box._center.x,_object._box._center.y,_object._box._center.z,1 };
	center = _object._worldTranslationMatrix* _object._objectTranslationMatrix * center;
	_object._box._center = glm::vec3(center);
	
	//Calculate Scene Matrix (Projection * View * Model) for world coordinates in column major order
	glm::mat4 SceneMatrix = _camera._projectionMatrix * _camera._viewMatrix * _object._worldTranslationMatrix * _object._worldModelMatrix * _object._objectTranslationMatrix * _object._objectModelMatrix;
	//Return Scene Matrix
	return SceneMatrix;
}

Object Scene::getObject() {
	return _object;
}
