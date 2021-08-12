#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up) 
	: MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	Orientation = glm::quat(0, 0, 0, -1);
	RightAngle = 0.f;
	UpAngle = 0.f;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ)
	: MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	Orientation = glm::quat(0, 0, 0, -1);
	RightAngle = 0.f;
	UpAngle = 0.f;
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	
	glm::quat reverseOrient = glm::conjugate(Orientation);
	glm::mat4 rot = glm::mat4_cast(reverseOrient);
	glm::mat4 translation = glm::translate(glm::mat4(1.0), -Position);

	return rot * translation;
}

void Camera::ProcessKeyboard(CameraDirection direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime * 3;

	glm::quat qF = Orientation * glm::quat(0, 0, 0, -1) * glm::conjugate(Orientation);
	glm::vec3 Front = { qF.x, qF.y, qF.z };
	glm::vec3 Right = glm::normalize(glm::cross(Front, glm::vec3(0, 1, 0)));

	if (direction == CameraDirection::FORWARD)
		Position += Front * velocity;

	if (direction == CameraDirection::BACKWARD)
		Position -= Front * velocity;

	if (direction == CameraDirection::LEFT)
		Position -= Right * velocity;

	if (direction == CameraDirection::RIGHT)
		Position += Right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	RightAngle += xoffset;
	UpAngle += yoffset;

	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.f && Zoom <= 45.f)
		Zoom -= yoffset;

	if (Zoom <= 1.f)
		Zoom = 1.f;

	if (Zoom >= 45.f)
		Zoom = 45.f;
}

void Camera::updateCameraVectors()
{
	glm::quat aroundY = glm::angleAxis(glm::radians(-RightAngle), glm::vec3(0, 1, 0));

	glm::quat aroundX = glm::angleAxis(glm::radians(UpAngle), glm::vec3(1, 0, 0));

	Orientation = aroundY * aroundX;
}
