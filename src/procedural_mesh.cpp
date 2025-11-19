#include "procedural_mesh.hpp"
#include <glad/glad.h>
procedural_mesh::procedural_mesh(const std::vector<vertex>& vert, const std::vector<unsigned int>& indi)
{
	verticies = vert;
	indicies = indi;
	
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

}
