#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include "EBO.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "Texture.h"
#include "Ray.h"
#include "BasicCamera.h"
#include "Scene.h"

// ray tracer information
const int BOUNCES = 3;
const int SAMPLES = 1;

// window dimensions
int window_width = 900;
int window_height = 900;

// vertices of a square (vertex pos, vertex uv coords)
GLfloat vertices[] =
{
	-1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // lower left corner,  (0,0)
	1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // lower right corner, (1,0)
	-1.0f, 1.0f, 0.0f,   0.0f, 1.0f, // upper left corner,  (0, 1)
	1.0f, 1.0f, 0.0f,    1.0f, 1.0f  // upper right corner, (1, 1)
};

GLuint indices[] =
{
	0, 1, 2, // lower left triangle
	2, 1, 3, // upper right triangle
};

// scene information
const glm::vec4 CLEAR_COLOR(0.07f, 0.13f, 0.17f, 1.0f);
const glm::vec3 SKY_COLOR(0.6f, 0.7f, 0.9f);
const glm::vec3 LIGHT_DIR(-1.0f, -1.0f, -1.0f);

struct HitPayload
{
	float HitDistance;
	glm::vec3 WorldPosition;
	glm::vec3 WorldNormal;
	int ObjectIndex;
};

HitPayload Miss(const Ray& ray)
{
	HitPayload payload;
	payload.ObjectIndex = -1;
	return payload;
}

HitPayload ClosestHit(const Scene& scene, const Ray& ray, float hitDist, int objectIndex)
{
	HitPayload payload;
	payload.HitDistance = hitDist;
	payload.ObjectIndex = objectIndex;

	const Sphere& closestSphere = scene.Spheres[objectIndex];

	glm::vec3 origin = ray.Origin - closestSphere.Position;

	payload.WorldPosition = origin + ray.Direction * hitDist;
	payload.WorldNormal= glm::normalize(payload.WorldPosition);

	payload.WorldPosition += closestSphere.Position;

	return payload;
}

// returns color for given ray
HitPayload TraceRay(const Scene& scene, const Ray& ray)
{
	int closestSphere = -1;
	float hitDist = FLT_MAX;

	// loop through all spheres in the scene to find closest intersection
	for (int i = 0; i < scene.Spheres.size(); i++)
	{
		const Sphere& sphere = scene.Spheres[i];

		glm::vec3 origin = ray.Origin - sphere.Position;
		
		// terms for quadratic formula
		float a = glm::dot(ray.Direction, ray.Direction);
		float b = 2.0f * glm::dot(origin, ray.Direction);
		float c = glm::dot(origin, origin) - sphere.Radius * sphere.Radius;

		// b^2 - 4ac (quadratic formula discriminant)
		float discriminant = b * b - 4.0f * a * c;

		// no intersection
		if (discriminant <= 0.0f)
			continue;

		// (-b +- sqrt(discriminant)) / 2a
		//float otherT = (-b + glm::sqrt(discriminant)) / (2.0f * a);
		float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);
		if (closestT > 0 && closestT < hitDist)
		{
			closestSphere = i;
			hitDist = closestT;
		}
	}

	// return miss payload on miss
	if (closestSphere < 0)
		return Miss(ray);

	// return hit payload on intersection
	return ClosestHit(scene, ray, hitDist, closestSphere);
}

glm::vec4 PerPixel(const Scene& scene, BasicCamera& camera, float x, float y)
{
	Ray ray;
	// ray origin is the cameras position
	ray.Origin = camera.position;
	ray.Direction = camera.GetRayDirection(x, y); // this is extremely costly

	glm::vec3 color(0.0f);

	float multiplier = 1.0f;
	for (int i = 0; i < BOUNCES; i++)
	{
		HitPayload payload = TraceRay(scene, ray);
		// return sky color on ray miss
		if (payload.ObjectIndex < 0)
		{
			color += SKY_COLOR * multiplier;
			break;
		}

		// basic lighting for sphere that the ray intersected with
		glm::vec3 light = glm::normalize(LIGHT_DIR);
		float lightIntensity = glm::max(glm::dot(payload.WorldNormal, -light), 0.0f);

		// get albedo of sphere that the ray intersected with
		const Sphere& closestSphere = scene.Spheres[payload.ObjectIndex];
		Material mat = scene.Materials[closestSphere.MaterialIndex];
		glm::vec3 sphereColor = mat.Albedo;
		sphereColor *= lightIntensity;

		// add sphere color to pixel
		color += sphereColor * multiplier;

		// reduce intensity of colors picked up with each bounce
		multiplier *= 0.35f;

		// set ray origin to be sphere intersection point
		ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.001f;
		glm::vec3 rand = glm::linearRand(glm::vec3(-0.5f), glm::vec3(0.5f));
		// set ray direction to be reflection of ray + some random offset
		ray.Direction = glm::reflect(ray.Direction, payload.WorldNormal + rand * mat.Roughness);
	}

	// clamp color rgb
	color = glm::vec3(
		glm::clamp(color.x, 0.0f, 1.0f),
		glm::clamp(color.y, 0.0f, 1.0f),
		glm::clamp(color.z, 0.0f, 1.0f)
	);

	return glm::vec4(color, 1.0f);
}

void setTexturePixels(GLubyte* pixels, const Scene& scene, BasicCamera& camera)
{
	for (int i = 0; i < window_height; i++)
	{
		for (int j = 0; j < window_width; j++)
		{
			// Convert i,j (our x and y coords) to range -1 -> 1 (clip coords!)
			float x = ((float)i / (window_width / 2)) - 1;
			float y = ((float)j / (window_height / 2)) - 1;

			// average the color of multiple samples to fake an
			// accumulation buffer (denoise image)
			glm::vec4 color(0.0f);
			for (int i = 0; i < SAMPLES; i++)
			{
				color += PerPixel(scene, camera, x, y);
			}
			color /= (float) SAMPLES;

			// Each pixel has RGBA values
			int index = (j + i * window_width) * 4;
			pixels[index] = static_cast<GLubyte>(color.r * 255.0f);
			pixels[index + 1] = static_cast<GLubyte>(color.g * 255.0f);
			pixels[index + 2] = static_cast<GLubyte>(color.b * 255.0f);
			pixels[index + 3] = static_cast<GLubyte>(color.a * 255.0f);
		}
	}
}

// update viewport on window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	window_width = width;
	window_height = height;

	// update viewport
	glViewport(0, 0, width, height);
}

int main()
{
	glfwInit();

	// tell GLFW what version of OpenGL we are using
	int const OPENGL_VERSION = 4;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION);
	// tell GLFW to use CORE profile - modern functions only
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a GLFW window
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "OpenGLRayTracing", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// set window as current context
	glfwMakeContextCurrent(window);

	// set framebuffer size callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// load GLAD so it configures to OpenGL
	gladLoadGL();

	// set viewport of OpenGL in the Window
	glViewport(0, 0, window_width, window_height);

	// generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// generates Vertex Array Object
	VAO VAO1;
	// bind VAO1 to Vertex Array Object
	VAO1.Bind();

	// generate Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// generate Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// link VBO to VAO
	// aPos layout location = 0
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	// aTexCoord layout location = 1
	VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// create a scene
	Scene scene;

	// create spheres and materials
	Sphere sphere1;
	sphere1.MaterialIndex = 0;
	Material mat1;
	mat1.Albedo = { 1.0f, 0.0f, 1.0f };

	// "floor"
	Sphere sphere2;
	sphere2.Position = { -10.0f, 0.0f, 0.0f };
	sphere2.Radius = 9.0f;
	sphere2.MaterialIndex = 1;
	Material mat2;
	mat2.Albedo = { 0.0f, 0.0f, 1.0f };
	mat2.Roughness = 0.05f;

	Sphere sphere3;
	sphere3.Position = { -0.25f, -1.25f, -2.0f };
	sphere3.MaterialIndex = 2;
	Material mat3;
	mat3.Albedo = { 1.0f, 0.0f, 0.0f };

	Sphere sphere4;
	sphere4.Position = { 1.25f, 1.0f, -1.0f };
	sphere4.MaterialIndex = 3;
	Material mat4;
	mat4.Albedo = { 0.0f, 1.0f, 0.0f };

	// add spheres to a scene
	scene.Spheres.push_back(sphere1);
	scene.Spheres.push_back(sphere2);
	scene.Spheres.push_back(sphere3);
	scene.Spheres.push_back(sphere4);
	scene.Materials.push_back(mat1);
	scene.Materials.push_back(mat2);
	scene.Materials.push_back(mat3);
	scene.Materials.push_back(mat4);

	// create a camera
	BasicCamera camera(45.0f, 0.01f, 100.0f, window_width, window_height);
	camera.SetPosition(glm::vec3(2.0f, 0.0f, 5.0f));
	camera.LookAt(glm::vec3(0.0f, 0.0f, 0.0f));

	// generate pixel data
	GLubyte* pixels = new GLubyte[window_width * window_height * 4];
	setTexturePixels(pixels, scene, camera);

	// create a texture
	Texture texture(pixels, window_width, window_height, GL_TEXTURE_2D, GL_TEXTURE0, GL_LINEAR, GL_RGBA, GL_UNSIGNED_BYTE);
	texture.LinkUni(shaderProgram, "sampler", 0);

	// set clear color
	glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, CLEAR_COLOR.a);

	// main while loop
	while (!glfwWindowShouldClose(window))
	{
		// clear back buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// tell OpenGL which shader program to use
		shaderProgram.Use();

		// bind Texture
		texture.Bind();

		// bind VAO 
		VAO1.Bind();

		// draw the quad
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// swap back buffer with the front buffer
		glfwSwapBuffers(window);

		// takes care of GLFW events
		glfwPollEvents();
	}

	// delete all objects we have created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	texture.Delete();
	shaderProgram.Delete();

	// delete GLFW window & GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}