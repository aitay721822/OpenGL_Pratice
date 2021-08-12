#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class CameraDirection
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float SPEED = 10.0f;
const float SENSITIVITY = 0.2f;
const float ZOOM = 45.0f;
class Camera
{
public:
	
	
	glm::vec3 Position;
	glm::quat Orientation;
	float RightAngle;
	float UpAngle;

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	Camera
	(
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
		glm::vec3 up = glm::vec3(0.f, 1.0f, 0.f)
	);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ);

	glm::mat4 GetViewMatrix();
	void ProcessKeyboard(CameraDirection direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	void ProcessMouseScroll(float yoffset);

private:
	void updateCameraVectors();
};

#endif
