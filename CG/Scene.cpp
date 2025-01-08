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
	//Apply Object Transformations
	//_object.Transform();
	//Calculate Scene Matrix (Projection * View * Model) for world coordinates in column major order
	glm::mat4 SceneMatrix = _camera._projectionMatrix * _camera._viewMatrix * _object._worldModelMatrix * _object._objectModelMatrix;
	//glm::mat4 xSceneMatrix = _camera._projectionMatrix * _camera._viewMatrix ;
	//Reset Matrices for object
	_object.ResetMatrices();
	//Return Scene Matrix
	return SceneMatrix;
}

Object Scene::getObject() {
	return _object;
}
