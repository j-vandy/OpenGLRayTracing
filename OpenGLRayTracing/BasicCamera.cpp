#include "BasicCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

BasicCamera::BasicCamera(float verticalFOV, float nearClip, float farClip, int viewport_width, int viewport_height) : verticalFOV(verticalFOV), nearClip(nearClip), farClip(farClip), viewport_width(viewport_width), viewport_height(viewport_height)
{
	position = glm::vec3(0.0f, 0.0f, 3.0f);
	forward = glm::vec3(0.0f, 0.0f, -1.0f);
	view = CalculateView();
	projection = CalculateProjection();
}

void BasicCamera::HandleViewportResize(int width, int height)
{
	viewport_width = width;
	viewport_height = height;
	projection = CalculateProjection();
}

void BasicCamera::LookAt(glm::vec3 target)
{
	forward = glm::normalize(target - position);
	view = CalculateView();
}

void BasicCamera::SetPosition(glm::vec3 pos)
{
	position = pos;
	view = CalculateView();
}

glm::vec3 BasicCamera::GetRayDirection(float x, float y)
{
	glm::vec4 target = glm::inverse(projection) * glm::vec4(x, y, 1, 1);
	return glm::vec3(glm::inverse(view) * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0)); // World space
}

glm::mat4 BasicCamera::CalculateView()
{
	return glm::lookAt(position, position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 BasicCamera::CalculateProjection()
{
	return glm::perspectiveFov(glm::radians(verticalFOV), (float)viewport_width, (float)viewport_height, nearClip, farClip);
}