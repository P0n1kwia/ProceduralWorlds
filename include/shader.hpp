#pragma once
#include <string>
#include <map>
#include <glm/glm.hpp>
class shader
{
public:
	shader(const std::string& vertexPath, const std::string& fragmentPath);
	void use();
	void CheckCompilationErrors(unsigned int shader, const std::string& type);

	void setFloat(const std::string& name, float value);
	void setMat4(const std::string& name, const glm::mat4& mat);
	void setVec3(const std::string name, const glm::vec3& vec);



	~shader();
	shader(const shader&) = delete;
	shader& operator=(const shader&) = delete;
private:
	unsigned int ID;
	std::map<std::string, unsigned int> uniformLocation;

	unsigned int GetUniformLocation(const std::string& name);
};