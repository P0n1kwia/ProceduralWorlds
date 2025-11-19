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
class procedural_mesh
{
public:
	procedural_mesh(const std::vector<vertex>& vert, const std::vector<unsigned int>& indicies);
	void Draw(const shader& shad);
	~procedural_mesh();
private:
	void SetupMesh();
	unsigned int VAO, VBO, EBO;
	std::vector<vertex> verticies;
	std::vector<unsigned int> indicies;

	
};