#ifndef BASIC_CAMERA_CLASS_H
#define BASIC_CAMERA_CLASS_H

#include <glm/glm.hpp>

class BasicCamera
{
public:
	// in world coords
	BasicCamera(float verticalFOV, float nearClip, float farClip, int viewport_width, int viewport_height);

	glm::vec3 position;
	void HandleViewportResize(int width, int height);
	void LookAt(glm::vec3 target);
	void SetPosition(glm::vec3 pos);
	glm::vec3 GetPostion() { return position; }
	glm::vec3 GetRayDirection(float x, float y);
private:
	float verticalFOV;
	float nearClip;
	float farClip;
	int viewport_width;
	int viewport_height;

	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 forward;

	glm::mat4 CalculateView();
	glm::mat4 CalculateProjection();
};

#endif