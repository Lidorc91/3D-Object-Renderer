#include "Object.h"
#include "Camera.h"
#include "Scene.h"
#include <vector>

class Scene {
public:
	std::vector<Object> objects;
	std::vector<Camera> cameras;
	Camera* viewer;

	//Add WORLD transoformations


	//Draw scene
	void Scene::DrawScene() {

	}

};
