#pragma once

#include <vector>
#include "Object.h"
#include "Camera.h"

class Scene
{
public:
    Object _object;
    Camera _camera;
	bool _hasObject;
    std::vector<glm::vec4> _axisPoints;
    std::set<pair<int, int>> _axisEdges;
    //void update(); // Update objects in the scene
    //void rasterize(std::vector<Pixel>& pixels); // Convert scene to pixel data
    Scene();
    void setObject(Wavefront_obj& obj);
    glm::mat4 GenerateScene();
    //std::vector<glm::mat4> Transformations; ADD WHEN IMPLEMENTING MULTIPLE OBJECTS
    Object getObject();
    void GenerateAxis();
private:
    //glm::mat4 _viewportMatrix;

    //void CalculateViewPortMatrix();
};

