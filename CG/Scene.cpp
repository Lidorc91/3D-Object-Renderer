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

	//Destructor
	Scene::~Scene() {
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
		/*
		glm::mat4 ViewMatrix = _viewer->_projectionMatrix * _viewer->_viewMatrix;
		for(Object& object : _objects) {
			//calculate Model-View Matrix
			glm::mat4 ModelViewMatrix = ViewMatrix * object._modelMatrix;
			
			for (glm::vec4 & point : object._meshModel._points) {
				//Transofrm each point
				glm::vec4 newPoint = point * ModelViewMatrix;
				//Perspective Divide (divide w) *I can drop z here too (no depth buffering) //TODO - FIX
				newPoint /= newPoint.w;
				//Viewport transform
				//newPoint = newPoint * _viewportMatrix;
				//Store new point
				point = newPoint;
			}
		}
		*/
		return SceneMatrix;
	}

	std::vector<Object>& Scene::getObjects() {
		return _objects;
	}
