
#include <shader.hpp>
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
shader::shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexCode, fragmentCode;
	std::ifstream vertexFile, fragmentFile;
	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		std::stringstream vertexSS, fragmentSS;
		vertexFile.open(vertexPath);
		vertexSS << vertexFile.rdbuf();
		vertexFile.close();

		fragmentFile.open(fragmentPath);
		fragmentSS << fragmentFile.rdbuf();
		fragmentFile.close();

		vertexCode = vertexSS.str();
		fragmentCode = fragmentSS.str();

	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "Failed  to open shader File! " << e.what() << "\n";
	}
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSource, nullptr);
	glCompileShader(vertex);
	CheckCompilationErrors(vertex, "VERTEX");

	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentSource, nullptr);
	glCompileShader(fragment);
	CheckCompilationErrors(fragment, "FRAGMENT");

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	CheckCompilationErrors(ID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);

}
void shader::use()
{
	glUseProgram(ID);
}
void shader::CheckCompilationErrors(unsigned int shader, const std::string& type)
{
	int success;
	char log[1024];
	if (type == "PROGRAM")
	{
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			std::cerr << "Failed to link program!\n";
			glGetProgramInfoLog(ID, 1024, nullptr, log);
			std::cerr << log << std::endl;
		}
	}
	else
	{
		if (type == "VERTEX")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				std::cerr << "Failed to complie Vertex Shader!\n";
				glGetProgramInfoLog(shader, 1024, nullptr, log);
				std::cerr << log << std::endl;
			}
		}
		
		else if (type == "FRAGMENT")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				std::cerr << "Failed to complie Fragment Shader!\n";
				glGetProgramInfoLog(shader, 1024, nullptr, log);
				std::cerr << log << std::endl;
			}
		}	
	}
}
void shader::setFloat(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}
void shader::setMat4(const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}
void shader::setVec3(const std::string name, const glm::vec3& vec)
{
	glUniform3fv(GetUniformLocation(name), 1, &vec[0]);
}
void shader::setMat3(const std::string& name, const glm::mat3& mat)
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}
shader::~shader()
{
	glDeleteProgram(ID);
}
unsigned int shader::GetUniformLocation(const std::string& name)
{
	if (uniformLocation.find(name) != uniformLocation.end())
	{
		return uniformLocation[name];
	}
	unsigned int location = glGetUniformLocation(ID, name.c_str());
	if (location == -1)
	{
		std::cerr << "Failed to find uniform location! SHADER_GET_UNIFORM_LOCATION()\n";
	}
	uniformLocation[name] = location;

	return location;
}
