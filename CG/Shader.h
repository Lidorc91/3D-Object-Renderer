#pragma once
#include "Object.h"
#include "Scene.h"

struct Pixel
{
	int  x, y; //pixel in screen coordinates
	unsigned int color; //RGBA color format - 4 components of 8 bits each - 0xAABBGGRR - AA alpha, BB blue, RR red
};

class Shader
{
public:
	Shader();

	void RenderFlatShading(const Scene& scene, std::vector<Pixel>& pixels , Object& tempObj, Object& worldObj, std::vector<float>& _zBuffer, int screenWidth, int screenHeight);
	void RenderGouraudShading(const Scene& scene, std::vector<Pixel>& pixels);
	void RenderPhongShading(const Scene& scene, std::vector<Pixel>& pixels);

	glm::vec3 CalculateIllumination(const Object& obj, const LightSource& light,const pair<std::array<int, 3>, glm::vec4>& faceNormal, glm::vec3 fragmentPosition,const Camera& camera);
	bool CheckBarycentricCoordinates(int xIndex, int yIndex, float v0x, float v0y, float v1x, float v1y, float v2x, float v2y, float denom);
};	

