#include "Shader.h"
#include <iostream>

Shader::Shader()
{
}

void Shader::RenderFlatShading(const Object& obj)
{
	std::cout << "Flat Shading" << std::endl;
}

void Shader::RenderGouraudShading(const Object& obj)
{
	std::cout << "Gouraud Shading" << std::endl;
}

void Shader::RenderPhongShading(const Object& obj)
{
	std::cout << "Phong Shading" << std::endl;
}