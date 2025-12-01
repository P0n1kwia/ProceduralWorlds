#include "turtle_interpreter.hpp"
#include <glm/gtc/matrix_transform.hpp>
turtleInterpreter::turtleInterpreter(float angle, float length, float width)
{
	this->angle = angle;
	this->length = length;
	this->width = width;
}

std::vector<glm::mat4> turtleInterpreter::Translate(const std::string& s)
{
	glm::mat4 currentTurtleMatrix = glm::mat4(1.0f);
	std::stack<glm::mat4> matrixStack;
	std::vector<glm::mat4> result;
	glm::vec3 cylinder = glm::vec3(width, length, width);
	

	result.reserve(s.length()/3);
	for (auto& ch : s)
	{
		glm::mat4 branchMatrix = glm::mat4(1.0f);
		switch (ch)
		{
		case 'F':
			branchMatrix = glm::scale(currentTurtleMatrix, cylinder);
			result.push_back(branchMatrix);
			currentTurtleMatrix =  glm::translate(currentTurtleMatrix, glm::vec3(0.0f, length, 0.0f));
			
			break;
		case '&':
			currentTurtleMatrix =  glm::rotate(currentTurtleMatrix, glm::radians(+angle), glm::vec3(1.0f, 0.0f, 0.0f));
			break;
		case '^':
			currentTurtleMatrix =  glm::rotate(currentTurtleMatrix, glm::radians(-angle), glm::vec3(1.0f, 0.0f, 0.0f));
			break;
		case '+':
			currentTurtleMatrix =  glm::rotate(currentTurtleMatrix, glm::radians(+angle), glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		case '-':
			currentTurtleMatrix =  glm::rotate(currentTurtleMatrix, glm::radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		case '\\':
			currentTurtleMatrix =  glm::rotate(currentTurtleMatrix, glm::radians(+angle), glm::vec3(0.0f, 0.0f, 1.0f));
			break;
		case '/':
			currentTurtleMatrix =  glm::rotate(currentTurtleMatrix, glm::radians(-angle), glm::vec3(0.0f, 0.0f, 1.0f));
			break;
		case '[':
			matrixStack.push(currentTurtleMatrix);
			break;
		case ']':
			currentTurtleMatrix = matrixStack.top();
			matrixStack.pop();
			break;
		}
	}
	return result;

}
