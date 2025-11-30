#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <shader.hpp>
#include <mesh_data.hpp>



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