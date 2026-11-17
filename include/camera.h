#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
	float yaw;
	float pitch;
	float fov;
	float max_fov;
	float sensitivity;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;

	Camera();
	Camera(float yaw, float pitch, float fov, float sensitivity,
			glm::vec3 position, glm::vec3 front, glm::vec3 up);

	void processMouseMovement(float xOffset, float yOffset);
	void processMouseWheel(float y);
	void handleCameraMovement(const float cameraSpeed, const bool* key_states);
};

#endif