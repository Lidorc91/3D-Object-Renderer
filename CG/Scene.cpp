#include "Scene.h"

	//Intialize viewer
	Camera* Scene::_viewer = nullptr;

	//Add WORLD transoformations


	//Constructor
	Scene::Scene(int width, int height) : _objects(), _cameras() {
		_screenWidth = width;
		_screenHeight = height;
		Camera* c = new Camera(_screenWidth/_screenHeight);
		_viewer = c;
		CalculateViewPortMatrix();
	}

	//Destructor
	Scene::~Scene() {
		for each (const Object & object in _objects) {
			delete &object;
		}
		for each (const Camera & camera in _cameras) {
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
			glm::mat4 ModelViewMatrix = _viewer->_projectionMatrix * _viewer->_viewMatrix * object._modelMatrix;
			//Perspective Divide (divide and drop w) *I can drop z here too (no depth buffering)
			ModelViewMatrix /= ModelViewMatrix[3];
			//Viewport transform
			glm::mat4 finalMatrix = _viewportMatrix * ModelViewMatrix;
		}
	}

	std::vector<Object> Scene::getObjects() {
		return _objects;
	}

	void Scene::CalculateViewPortMatrix() {
		float halfWidth = _screenWidth / 2.0f;
		float halfHeight = _screenHeight / 2.0f;

		_viewportMatrix[0][0] = halfWidth;  // Scale X
		_viewportMatrix[1][1] = halfHeight; // Scale Y
		_viewportMatrix[3][0] = halfWidth;  // Translate X
		_viewportMatrix[3][1] = halfHeight; // Translate Y
	}