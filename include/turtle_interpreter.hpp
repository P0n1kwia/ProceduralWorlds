#pragma once
#include <stack>
#include <glm/glm.hpp>
#include <Lsystems_generator.hpp>
#include <string>
class turtleInterpreter
{
public:
	turtleInterpreter(float angle, float length, float width = 1.0f);
	std::vector<glm::mat4> Translate(const std::string& s);
private:
	float angle;
	float length;
	float width; // width of a branch
	
};