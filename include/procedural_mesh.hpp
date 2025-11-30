#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <shader.hpp>
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

class proceduralMesh
{
public:
	proceduralMesh(const meshData& data);
	void Draw(const shader& shad);
	~proceduralMesh();

private:
	void SetupMesh();
	unsigned int VAO, VBO, EBO;
	std::vector<vertex> verticies;
	std::vector<unsigned int> indicies;

	
};