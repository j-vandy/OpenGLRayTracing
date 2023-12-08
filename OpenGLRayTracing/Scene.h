#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include <glm/glm.hpp>
#include <vector>;

struct Material
{
	glm::vec3 Albedo{ 1.0f };
	float Roughness = 0.0f;
	float Metallic = 0.0f;
};

struct Sphere
{
	glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
	float Radius = 0.5f;
	int MaterialIndex;
};

struct Scene
{
	std::vector<Sphere> Spheres;
	std::vector<Material> Materials;
};

#endif