// ===== Application.hpp =====
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>
#include <glm/glm.hpp>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Scene.hpp"
#include "BasicScene.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "SkyDome.hpp"

class Application {
public:
	Application(int width, int height, const std::string& title);
	~Application();
	int Run();

private:
	void InitWindow(int w, int h, const std::string& title);
	void InitScene();
	void InitCrosshair();
	void ProcessInput();
	void Update();
	void Render();
	void OnMouseMove(double xpos, double ypos);

	int width, height;
	std::string title;
	GLFWwindow* window;

	Camera* camera;
	Scene* scene;
	SkyDome* sky;

	// mouse control
	float yaw, pitch, lastX, lastY;
	bool firstMouse;

	// crosshair
	Shader* crosshairShader;
	unsigned int crossVAO, crossVBO;
};

#endif // APPLICATION_HPP
