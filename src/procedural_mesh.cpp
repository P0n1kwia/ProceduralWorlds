#include "procedural_mesh.hpp"
#include <glad/glad.h>
procedural_mesh::procedural_mesh(const meshData& data)
{
	verticies = data.verticies;
	indicies = data.indicies;
	
	SetupMesh();
}
void procedural_mesh::Draw(const shader& shad)
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
procedural_mesh::~procedural_mesh()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

}

void procedural_mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * verticies.size(), verticies.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicies.size(), indicies.data(), GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(vertex, texCoord));

	glBindVertexArray(0);

}
