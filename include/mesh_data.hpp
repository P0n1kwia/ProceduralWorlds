#pragma once
#include <glm/glm.hpp>
#include <vector>
struct vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};
struct meshData
{
	std::vector<vertex>	verticies;
	std::vector<unsigned int> indicies;
};