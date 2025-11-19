#pragma once
#include <string>
#include <map>

class shader
{
public:
	shader(const std::string& vertexPath, const std::string& fragmentPath);
	void use();
	void CheckCompilationErrors(unsigned int shader, const std::string& type);


	~shader();
	shader(const shader&) = delete;
	shader& operator=(const shader&) = delete;
private:
	unsigned int ID;
	std::map<std::string, unsigned int> uniformLocation;

	unsigned int GetUniformLocation(const std::string& name);
};