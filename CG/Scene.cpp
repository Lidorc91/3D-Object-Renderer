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
		_axisPoints.clear();
		_axisEdges.clear();
		
	}

	//Generate ALL transformations (before viewport transformations)
	glm::mat4 Scene::GenerateScene() {
		//Apply Object Transformations
		_object.Transform();
		GenerateAxis();
		//Calculate Scene Matrix (Projection * View * Model) for world coordinates in column major order
		glm::mat4 SceneMatrix = _camera._projectionMatrix * _camera._viewMatrix * _object._modelMatrix;
		glm::mat4 xSceneMatrix = _camera._projectionMatrix * _camera._viewMatrix ;
		//Reset Matrices for object
		_object.ResetMatrices();
		//Return Scene Matrix
		return SceneMatrix;
		return xSceneMatrix;
	}
	void Scene::GenerateAxis() {
		//Create world axis
		//center of object
		_axisPoints.emplace_back(glm::vec4(_object._box._center.x, _object._box._center.y, _object._box._center.z, 1.0f));
		//X-axis
		_axisPoints.emplace_back(glm::vec4(_object._box._center.x +0.3, _object._box._center.y , _object._box._center.z , 1.0f));
		//Y-axis
		_axisPoints.emplace_back(glm::vec4(_object._box._center.x , _object._box._center.y + 0.3, _object._box._center.z , 1.0f));
		//Z-axis
		_axisPoints.emplace_back(glm::vec4(_object._box._center.x , _object._box._center.y , _object._box._center.z + 0.3, 1.0f));

		//Create edges
		_axisEdges.insert({ 0,1 });
		_axisEdges.insert({ 0,2 });
		_axisEdges.insert({ 0,3 });

		
	}
	Object Scene::getObject() {
		return _object;
	}
