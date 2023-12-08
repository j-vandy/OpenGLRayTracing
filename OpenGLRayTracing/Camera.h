#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glm/glm.hpp>

class Camera
{
public:
	// in world coords
	Camera(float verticalFOV, float nearClip, float farClip, int viewport_width, int viewport_height);

	void HandleViewportResize(int width, int height);
	void HandleKeyInput(double deltaTime, int action, int key);
	void HandleMousePositionInput(double deltaTime, double xpos, double ypos);
	glm::vec3 GetPosition() const { return position; }
	glm::vec3 GetRayDirection(float x, float y);
private:
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 position;
	glm::vec3 forward;
	float verticalFOV;
	float nearClip;
	float farClip;
	glm::vec2 previous_mouse_pos;
	int viewport_width;
	int viewport_height;
	bool W_isDown;
	bool A_isDown;
	bool S_isDown;
	bool D_isDown;
	bool Ctr_isDown;
	float movement_speed;
	float rotation_speed;

	void UpdateKeyStatus(int action, int key);
	glm::mat4 CalculateView();
	glm::mat4 CalculateProjection();
};


#endif
