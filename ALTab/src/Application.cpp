#include "Application.hpp"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// Constructor
Application::Application(int w, int h, const std::string& title)
	: width(w), height(h), title(title),
	window(nullptr), camera(nullptr), scene(nullptr),
	yaw(-90.0f), pitch(0.0f), lastX(0.0f), lastY(0.0f), firstMouse(true),
	crosshairShader(nullptr), crossVAO(0), crossVBO(0)
{
	InitWindow(width, height, title);
	InitScene();
	InitCrosshair();
}

// Destructor
Application::~Application() {
	delete crosshairShader;
	glDeleteVertexArrays(1, &crossVAO);
	glDeleteBuffers(1, &crossVBO);
	delete scene;
	delete camera;
	if (window) {
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}

// GLFW + GLAD setup
void Application::InitWindow(int w, int h, const std::string& title) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
	if (!window) { std::cerr << "Failed to create window\n"; std::exit(EXIT_FAILURE); }
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to init GLAD\n"; std::exit(EXIT_FAILURE);
	}
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y) {
		static_cast<Application*>(glfwGetWindowUserPointer(win))->OnMouseMove(x, y);
		});
	glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int sc, int act, int mods) {
		if (key == GLFW_KEY_ESCAPE && act == GLFW_PRESS)
			glfwSetWindowShouldClose(w, GLFW_TRUE);
		});
}

// Scene setup
void Application::InitScene() {
	// Camera above origin looking down slightly
	float aspect = static_cast<float>(width) / static_cast<float>(height);
	camera = new Camera({ 0,3,3 }, { 0,0,0 }, { 0,1,0 }, 45.0f, aspect, 0.1f, 100.0f);

	// Basic terrain scene
	scene = new BasicScene(10.0f, 100);
	scene->Init();
}

// Crosshair VAO/VBO + shader
void Application::InitCrosshair() {
	crosshairShader = new Shader("shaders/crosshair.vert", "shaders/crosshair.frag");
	float s = 5.0f;
	float hx = s / float(width) * 2.0f;
	float hy = s / float(height) * 2.0f;
	float verts[] = { -hx,0,  hx,0,  0,-hy,  0,hy };
	glGenVertexArrays(1, &crossVAO);
	glGenBuffers(1, &crossVBO);
	glBindVertexArray(crossVAO);
	glBindBuffer(GL_ARRAY_BUFFER, crossVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindVertexArray(0);
}

int Application::Run() {
	while (!glfwWindowShouldClose(window)) {
		ProcessInput();
		Update();
		Render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

void Application::ProcessInput() {
	// (none for now)
}

void Application::Update() {
	float delta = static_cast<float>(glfwGetTime());
	scene->Update(delta);
}

void Application::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 vp = camera->GetProjection() * camera->GetView();
	scene->Render(vp);

	// Draw crosshair
	glDisable(GL_DEPTH_TEST);
	crosshairShader->Use();
	glm::mat4 ortho = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
	crosshairShader->SetUniform("uProjection", ortho);
	glBindVertexArray(crossVAO);
	glLineWidth(2.0f);
	glDrawArrays(GL_LINES, 0, 4);
	glLineWidth(1.0f);
	glEnable(GL_DEPTH_TEST);
}

void Application::OnMouseMove(double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos; lastY = ypos; firstMouse = false;
	}
	float dx = (xpos - lastX) * 0.1f;
	float dy = (lastY - ypos) * 0.1f;
	lastX = xpos; lastY = ypos;
	yaw += dx; pitch += dy;
	pitch = glm::clamp(pitch, -89.0f, 89.0f);
	glm::vec3 dir{
		cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
		sin(glm::radians(pitch)),
		sin(glm::radians(yaw)) * cos(glm::radians(pitch))
	};
	camera->SetTarget(camera->GetPosition() + glm::normalize(dir));
}