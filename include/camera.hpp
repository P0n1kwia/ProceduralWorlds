#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};
class camera
{
public:
	camera(const glm::vec3& pos);
	glm::mat4 GetViewMatrix();
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset);
	glm::vec3 GetPosition() const;
	void SetMovementSpeed(float speed);

private:
	void UpdateCameraVectors();
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float movementSpeed;
	float yaw, pitch;
	float mouseSensitivity;
};