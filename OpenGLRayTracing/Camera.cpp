#include "Camera.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

Camera::Camera(float verticalFOV, float nearClip, float farClip, int viewport_width, int viewport_height) : verticalFOV(verticalFOV), nearClip(nearClip), farClip(farClip), viewport_width(viewport_width), viewport_height(viewport_height)
{
	// world coords
	position = glm::vec3(0.0f, 0.0f, 3.0f);
	forward = glm::vec3(0.0f, 0.0f, -1.0f);
	previous_mouse_pos = glm::vec2(0.0f);

	W_isDown = false;
	A_isDown = false;
	S_isDown = false;
	D_isDown = false;
	Ctr_isDown = false;

	movement_speed = 1.0f;
	rotation_speed = 1.0f;

	view = CalculateView();
	projection = CalculateProjection();
}

void Camera::HandleViewportResize(int width, int height)
{
	viewport_width = width;
	viewport_height = height;

	projection = CalculateProjection();
}

void Camera::HandleKeyInput(double deltaTime, int action, int key)
{
	UpdateKeyStatus(action, key);

	glm::vec3 rightDir = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));
	rightDir = glm::normalize(rightDir);

	bool moved = false;

	if (W_isDown)
	{
		position += forward * movement_speed * (float)deltaTime;
		moved = true;
	}
	if (A_isDown)
	{
		position -= rightDir * movement_speed * (float)deltaTime;
		moved = true;
	}
	if (S_isDown)
	{
		position -= forward * movement_speed * (float)deltaTime;
		moved = true;
	}
	if (D_isDown)
	{
		position += rightDir * movement_speed * (float)deltaTime;
		moved = true;
	}

	if (moved)
	{
		view = CalculateView();
	}
}

void Camera::HandleMousePositionInput(double deltaTime, double xpos, double ypos)
{
	if (!Ctr_isDown)
		return;

	glm::vec3 rightDir = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));
	rightDir = glm::normalize(rightDir);

	glm::vec2 mouse_pos(xpos, ypos);
	glm::vec2 delta = mouse_pos - previous_mouse_pos;
	previous_mouse_pos = mouse_pos;

	bool moved = false;

	if (delta.x != 0.0f || delta.y != 0.0f)
	{
		float pitch = delta.y * rotation_speed * (float)deltaTime;
		float yaw = delta.x * rotation_speed * (float)deltaTime;

		glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitch, rightDir), glm::angleAxis(-yaw, glm::vec3(0.0f, 1.0f, 0.0f))));
		forward = glm::rotate(q, forward);

		moved = true;
	}

	if (moved)
	{
		view = CalculateView();
	}
}

glm::vec3 Camera::GetRayDirection(float x, float y)
{
	glm::vec4 target = glm::inverse(projection) * glm::vec4(x, y, 1, 1);
	return glm::vec3(glm::inverse(view) * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0)); // World space
}

void Camera::UpdateKeyStatus(int action, int key)
{
	switch (key)
	{
	case 87: // W
		if (action == GLFW_PRESS) {
			W_isDown = true;
		}
		else if (action == GLFW_RELEASE) {
			W_isDown = false;
		}
		break;
	case 65: // A
		if (action == GLFW_PRESS) {
			A_isDown = true;
		}
		else if (action == GLFW_RELEASE) {
			A_isDown = false;
		}
		break;
	case 83: // S
		if (action == GLFW_PRESS) {
			S_isDown = true;
		}
		else if (action == GLFW_RELEASE) {
			S_isDown = false;
		}
		break;
	case 68: // D
		if (action == GLFW_PRESS) {
			D_isDown = true;
		}
		else if (action == GLFW_RELEASE) {
			D_isDown = false;
		}
		break;
	case 341: // Ctr
		if (action == GLFW_PRESS) {
			Ctr_isDown = true;
		}
		else if (action == GLFW_RELEASE) {
			Ctr_isDown = false;
		}
		break;
	}
}
glm::mat4 Camera::CalculateView()
{
	return glm::lookAt(position, position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::CalculateProjection()
{
	return glm::perspectiveFov(glm::radians(verticalFOV), (float)viewport_width, (float)viewport_height, nearClip, farClip);
}
