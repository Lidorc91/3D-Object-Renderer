#include "Scene.h"

	//Intialize viewer
	Camera* Scene::_viewer = nullptr;
	//Add WORLD transoformations

	//Constructor
	Scene::Scene() : _objects(), _cameras() {
		Camera c;
		_cameras.push_back(c);
		_viewer = &_cameras.at(0);
	}

	// Add an object to the scene
	void Scene::addObject(Object& obj) {
		_objects.push_back(obj);
	}

	// Add a camera to the scene
	void Scene::addCamera(Camera& cam) {
		_cameras.push_back(cam);
	}

	//Generate ALL transformations (before viewport transformations)
	glm::mat4 Scene::GenerateScene() {
		//curently for 1 object only !
		glm::mat4 SceneMatrix = _viewer->_projectionMatrix * _viewer->_viewMatrix * this->_objects.at(0)._modelMatrix;

		//Reset Matrices for object and camera
		this->_objects.at(0).ResetMatrices();
		this->_cameras.at(0).ResetMatrices();

		return SceneMatrix;
	}

	std::vector<Object>& Scene::getObjects() {
		return _objects;
	}
