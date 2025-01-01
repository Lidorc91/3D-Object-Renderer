#include "Scene.h"

	//Add WORLD transoformations

	//Constructor
	Scene::Scene() {
		_hasObject = false;
	}

	// Add an object to the scene
	void Scene::setObject(Object& obj) {
		_object = &obj;
		_hasObject = true;
	}

	//Generate ALL transformations (before viewport transformations)
	glm::mat4 Scene::GenerateScene() {
		//Apply Object Transformations
		_object->Transform();
		//curently for 1 object only !
		glm::mat4 SceneMatrix = _camera._projectionMatrix * _camera._viewMatrix * _object->_modelMatrix;

		//Reset Matrices for object and camera
		_object->ResetMatrices();
		_camera.ResetMatrices();

		return SceneMatrix;
	}

	Object& Scene::getObject() {
		return *_object;
	}
