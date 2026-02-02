#include "skybox.hpp"
#include "skybox.hpp"
#include <glad/gl.h>

skybox::skybox(const std::vector<std::string>& boxFaces)
{

}

void skybox::draw(shader& shad)
{
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

skybox::~skybox()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}
