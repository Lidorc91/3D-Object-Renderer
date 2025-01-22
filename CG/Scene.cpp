#include "Scene.h"

//Add WORLD transoformations

//Constructor
Scene::Scene() : _object() {
	_hasObject = false;
	_lightSource1 = LightSource(LightType::Directional);
	_lightSourceOptional = LightSource(LightType::Point);
	_lightSourceOptional._enabled = false; //default optional light is off
	_ambientLight = LightSource(LightType::Ambient);
}

// Add an object to the scene
void Scene::setObject(Wavefront_obj& obj) {
	_object.ReadFile(obj);
	_hasObject = true;
}

//Generate ALL transformations (before viewport transformations)
glm::mat4 Scene::GenerateScene() {	
	//Calculate Scene Matrix (Projection * View * Model) for world coordinates in column major order
	glm::mat4 SceneMatrix = _camera._projectionMatrix * _camera._viewMatrix * _object._worldTranslationMatrix * _object._worldModelMatrix * _object._objectTranslationMatrix * _object._objectModelMatrix;
	//Return Scene Matrix
	return SceneMatrix;
}

Object Scene::getObject() {
	return _object;
}
