#include "camera.h"

#include <SDL3/SDL.h>

Camera::Camera()
{
	yaw = -90.0f;
	pitch = 0.0f;
	fov = 45.0f;
	max_fov = 45.0f;
	sensitivity = 0.1f;

	position = glm::vec3(0.0f, 0.0f, 3.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::Camera(float yaw, float pitch, float fov, float sensitivity,
	glm::vec3 position, glm::vec3 front, glm::vec3 up)
{
	yaw = yaw;
	pitch = pitch;
	fov = fov;
	max_fov = fov;
	sensitivity = sensitivity;
	position = position;
	front = front;
	up = up;
}

void Camera::processMouseMovement(float xOffset, float yOffset)
{
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
		pitch = 89.0f;

	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(direction);

}

void Camera::processMouseWheel(float y)
{
	fov -= y;

	if (fov < 1.0f) fov = 1.0f;
	if (fov > max_fov) fov = max_fov;
}

void Camera::handleCameraMovement(const float cameraSpeed, const bool* key_states)
{
	if (key_states[SDL_SCANCODE_W])
		position += cameraSpeed * front;

	if (key_states[SDL_SCANCODE_S])
		position -= cameraSpeed * front;

	if (key_states[SDL_SCANCODE_A])
		position -= glm::normalize(glm::cross(front, up)) * cameraSpeed;

	if (key_states[SDL_SCANCODE_D])
		position += glm::normalize(glm::cross(front, up)) * cameraSpeed;
}