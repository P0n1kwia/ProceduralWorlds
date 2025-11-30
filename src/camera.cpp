#include <Camera.hpp>

Camera::Camera(const glm::vec3& pos)
{
	position = pos;
	yaw = -90.0f;
	pitch = 0.0f;
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	movementSpeed = 5.5f;
	mouseSensitivity = 0.1f;
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	switch (direction)
	{
	case Camera_Movement::FORWARD:
		position += front * movementSpeed * deltaTime;
		break;
	case Camera_Movement::BACKWARD:
		position -= front * movementSpeed * deltaTime;
		break;
	case Camera_Movement::LEFT:
		position -= right * movementSpeed * deltaTime;
		break;
	case Camera_Movement::RIGHT:
		position += right * movementSpeed * deltaTime;
		break;
	}
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;
	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}


	UpdateCameraVectors();
}

glm::vec3 Camera::GetPosition() const
{
	return position;
}

void Camera::SetMovementSpeed(float speed)
{
	movementSpeed = speed;
}

void Camera::UpdateCameraVectors()
{
	front.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
	front.y = std::sin(glm::radians(pitch));
	front.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::cross(front, worldUp);
	right = glm::normalize(right);

	up = glm::cross(right, front);
	up = glm::normalize(up);


}


