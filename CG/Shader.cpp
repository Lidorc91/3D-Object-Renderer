#include "Shader.h"
#include <iostream>

Shader::Shader()
{
}

void Shader::RenderFlatShading(const Scene& scene)
{
	//aliases for ease of use
	const Object& obj = scene._object;
	const Camera& camera = scene._camera;

	//for each face in object calculate illumination
	for (const auto& faceNormal : obj._meshModel._faceNormals) {
		//Calculate Centroid
		glm::vec3 centroid = (obj._meshModel._points[faceNormal.first[0]] + obj._meshModel._points[faceNormal.first[1]] + obj._meshModel._points[faceNormal.first[2]]) / 3.0f;
		//Calculate Illumination
		glm::vec3 I = CalculateIllumination(obj, scene._lightSource1, faceNormal, centroid, camera) + CalculateIllumination(obj, scene._lightSourceOptional, faceNormal, centroid, camera);
		//Draw face (add to pixel vector)

	}
	
}

void Shader::RenderGouraudShading(const Scene& scene)
{
	std::cout << "Gouraud Shading" << std::endl;
}

void Shader::RenderPhongShading(const Scene& scene)
{
	std::cout << "Phong Shading" << std::endl;
}

//Calculate illumination equation
//I = Ia + Id + Is
//Ia = Ka * Ia
//Id = Kd * Id * max(0, N.L)
//Is = Ks * Is * max(0, N.H)^n
//N = Normal
//L = Light Direction
//H = Halfway vector
//Ka, Kd, Ks = Ambient, Diffuse, Specular reflection coefficients
//Ia, Id, Is = Ambient, Diffuse, Specular light intensities
//n = Shininess coefficient

glm::vec3 Shader::CalculateIllumination(const Object& obj, const LightSource& light,const pair<std::array<int, 3>, glm::vec4>& faceNormal,glm::vec3 fragmentPosition,const Camera& camera)
{
	if (!light._enabled)
		return glm::vec3(0.0f);

	glm::vec3 I = glm::vec3(0.0f);
	switch (light._type)
	{
	case LightType::Ambient:
		//Calculate Ambient Light
		I += calculateAmbient(obj._material, light);
		break;
	case LightType::Point:
		//Calculate Diffuse Light
		I += calculateDiffuse(obj._material, light, faceNormal.second, fragmentPosition);
		//Calculate Specular Light
		glm::vec3 viewDir = glm::normalize(camera._eye - fragmentPosition);		
		I += calculateSpecular(obj._material, light, faceNormal.second, viewDir, fragmentPosition);
		break;
	case LightType::Directional:
		//Calculate Diffuse Light
		I += calculateDiffuse(obj._material, light, faceNormal.second, fragmentPosition);
		//Calculate Specular Light
		break;

	default:
		break;
	}
	return I;
}

glm::vec3 calculateAmbient(const Material& material, const LightSource& light) {
	return material._ambient * light._color ;
}

glm::vec3 calculateDiffuse(const Material& material, const LightSource& light, const glm::vec3& normal, const glm::vec3& fragmentPosition) {
	glm::vec3 lightDir; 

	if (light._type == LightType::Point) {
		lightDir = glm::normalize((glm::vec3)light._position - fragmentPosition); // Vector from surface to light
	}
	else if (light._type == LightType::Directional) {
		lightDir = glm::normalize(-light._direction); // Invert light's direction
	}
	else {
		return glm::vec3(0.0f); // Ambient lights don't contribute to diffuse
	}

	float diffuseStrength = glm::max(glm::dot(normal, lightDir), 0.0f); // Clamp to [0, 1]
	return material._diffuse * light._color * diffuseStrength;
}

glm::vec3 calculateSpecular(const Material& material, const LightSource& light,	const glm::vec3& normal, const glm::vec3& viewDir, const glm::vec3& fragmentPosition) {
	glm::vec3 lightDir;

	if (light._type == LightType::Point) {
		lightDir = glm::normalize((glm::vec3)light._position - fragmentPosition);
	}
	else if (light._type == LightType::Directional) {
		lightDir = glm::normalize(-light._direction);
	}
	else {
		return glm::vec3(0.0f); // Ambient lights don't contribute to specular
	}

	glm::vec3 reflectDir = glm::reflect(-lightDir, normal); // Reflection vector
	float spec = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), material._shininess);
	return material._specular * light._color * spec;
}