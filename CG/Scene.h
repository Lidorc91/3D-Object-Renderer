#pragma once

#include <vector>
#include "Object.h"
#include "Camera.h"

class Scene
{
public:
    std::vector<Object> _objects;
    std::vector<Camera> _cameras;
    static Camera* _viewer;

    //void update(); // Update objects in the scene
    //void rasterize(std::vector<Pixel>& pixels); // Convert scene to pixel data
    Scene();
    ~Scene();
    void addObject(Object& obj);
    void addCamera(Camera& cam);
    void GenerateScene();
    std::vector<Object>& getObjects();

private:
    //glm::mat4 _viewportMatrix;

    //void CalculateViewPortMatrix();
};

