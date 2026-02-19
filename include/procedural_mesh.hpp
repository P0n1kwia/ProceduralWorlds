#pragma once

#include <vector>
#include <shader.hpp>
#include <mesh_data.hpp>
#include <cstddef>


class proceduralMesh
{
public:
	proceduralMesh(const meshData& data);
	void Draw(const shader& shad);
	~proceduralMesh();
	unsigned int GetVBO() const { return VBO; }
	std::ptrdiff_t GetVBOSize()  const { return vboSize; }
	int GetLineVerts() const { return lineVerts; }

private:
	void SetupMesh();
	unsigned int VAO, VBO, EBO;
	std::vector<vertex> verticies;
	std::vector<unsigned int> indicies;

	std::ptrdiff_t vboSize = 0;
	int lineVerts = 0;
};