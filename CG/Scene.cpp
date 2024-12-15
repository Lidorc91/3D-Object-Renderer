#include "Scene.h"

	//Intialize viewer
	Camera* Scene::_viewer = nullptr;

	//Add WORLD transoformations


	//Constructor
	Scene::Scene() : _objects(), _cameras() {
		Camera* c = new Camera();
		_viewer = c;
	}

	//Destructor
	Scene::~Scene() {
		for each (Object & object in _objects) {
			delete &object;
		}
		for each (Camera & camera in _cameras) {
			delete &camera;
		}
	}

	// Add an object to the scene
	void Scene::addObject(const Object& obj) {
		_objects.push_back(obj);
	}

	// Add a camera to the scene
	void Scene::addCamera(const Camera& cam) {
		_cameras.push_back(cam);
	}

	//Draw scene
	void Scene::DrawScene() {
		
		for each (const Object & object in _objects) {
			//calculate Model-View Matrix
			//_ModelViewMatrix = _viewportMatrix * _projectionMAtrix *_viewer->_viewMatrix * object._modelMatrix;

		}
	}

	std::vector<Object> Scene::getObjects() {
		return _objects;
	}
