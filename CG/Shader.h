#pragma once
#include "Object.h"
#include "Scene.h"

class Shader
{
public:
	Shader();

	void RenderFlatShading(const Scene& scene);
	void RenderGouraudShading(const Scene& scene);
	void RenderPhongShading(const Scene& scene);

	glm::vec3 CalculateIllumination(const Object& obj, const LightSource& light,const pair<std::array<int, 3>, glm::vec4>& faceNormal, glm::vec3 fragmentPosition,const Camera& camera);
};	

