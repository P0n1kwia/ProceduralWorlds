#pragma once
#include <turtle_interpreter.hpp>
#include <shader.hpp>
#include <mesh_data.hpp>
class instancedMesh
{
public:
	instancedMesh(const meshData& geometry, const std::vector<glm::mat4>& instanceMatricies);
	~instancedMesh();
	void Draw(shader& shad);

private:
	unsigned int VBO,EBO, VAO, instanceVBO;
	unsigned int indexCount;
	unsigned int instanceCount;
	void SetupMesh(const meshData& geometry, const std::vector<glm::mat4>& instanceMatricies);
};