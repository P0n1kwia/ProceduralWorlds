#include "instanced_mesh.hpp"
#include <glad/glad.h>
instancedMesh::instancedMesh(const meshData& geometry, const std::vector<glm::mat4>& instanceMatricies)
{

	indexCount = geometry.indicies.size();
	instanceCount = instanceMatricies.size();
	SetupMesh(geometry, instanceMatricies);
}

instancedMesh::~instancedMesh()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &instanceVBO);
	glDeleteVertexArrays(1, &VAO);
}

void instancedMesh::Draw(shader& shad)
{
	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, instanceCount);
	glBindVertexArray(0);
}

void instancedMesh::SetupMesh(const meshData& geometry, const std::vector<glm::mat4>& instanceMatricies)
{
	float sizeMat4 = sizeof(glm::mat4);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &instanceVBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * geometry.verticies.size(), geometry.verticies.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * geometry.indicies.size(), geometry.indicies.data(), GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,sizeof(vertex), (void*)offsetof(vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, texCoord));
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, instanceMatricies.size() * sizeof(glm::mat4), instanceMatricies.data(), GL_STATIC_DRAW);
	int vec4Size = sizeof(glm::vec4);
	for (int i = 0; i < 4; i++)
	{
		glEnableVertexAttribArray(3 + i);
		glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * vec4Size));
		glVertexAttribDivisor(3 + i, 1);
	}
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


