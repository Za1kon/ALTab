// ===== Application.cpp =====
#include "Application.hpp"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <thread>
#include <chrono>

Application::Application(int w, int h, const std::string& title)
	: width(w), height(h), title(title),
	window(nullptr), camera(nullptr), scene(nullptr), sky(nullptr),
	yaw(-90.0f), pitch(0.0f), lastX(0.0f), lastY(0.0f), firstMouse(true),
	crosshairShader(nullptr), crossVAO(0), crossVBO(0),
	postShader(nullptr), postVAO(0), postVBO(0),
	sharpenFBO(0), sharpenTexture(0), sharpenRBO(0),
	msaaFBO(0), msaaColorRBO(0), msaaDepthRBO(0),
	sharpness(0.5f)
{
	InitWindow(width, height, title);
	InitScene();
	InitCrosshair();
	InitPostProcessing();
}

Application::~Application() {
	delete crosshairShader;
	delete postShader;
	glDeleteVertexArrays(1, &crossVAO);
	glDeleteBuffers(1, &crossVBO);
	glDeleteVertexArrays(1, &postVAO);
	glDeleteBuffers(1, &postVBO);
	delete scene;
	delete camera;
	delete sky;
	if (window) {
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}

void Application::InitWindow(int w, int h, const std::string& title) {
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
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
	glEnable(GL_MULTISAMPLE);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y) {
		static_cast<Application*>(glfwGetWindowUserPointer(win))->OnMouseMove(x, y);
		});
	glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int sc, int act, int mods) {
		if (key == GLFW_KEY_ESCAPE && act == GLFW_PRESS)
			glfwSetWindowShouldClose(w, GLFW_TRUE);
		});
	lastTime = glfwGetTime();
	fpsCapOption = 2;  // Cambiá este valor de 1 a 5 para elegir FPS
	switch (fpsCapOption) {
	case 1: invFrameTime = std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(1.0 / 60.0));  break;
	case 2: invFrameTime = std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(1.0 / 144.0)); break;
	case 3: invFrameTime = std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(1.0 / 240.0)); break;
	case 4: invFrameTime = std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(1.0 / 360.0)); break;
	case 5: invFrameTime = std::chrono::steady_clock::duration::zero(); break; // unlimited
	}
	frameCount = 0;
}

void Application::InitScene() {
	float aspect = static_cast<float>(width) / static_cast<float>(height);
	camera = new Camera({ 0,1,0 }, { 0,0,0 }, { 0,1,0 }, 45.0f, aspect, 0.1f, 100.0f);

	scene = new BasicScene(2.0f, 20);
	scene->Init();

	sky = new SkyDome("assets/skydome.hdr", 100.0f);

	// === Crear framebuffer multisample ===
	glGenFramebuffers(1, &msaaFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, msaaFBO);

	// Color renderbuffer multisample
	glGenRenderbuffers(1, &msaaColorRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, msaaColorRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, msaaColorRBO);

	// Depth-stencil renderbuffer multisample
	glGenRenderbuffers(1, &msaaDepthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, msaaDepthRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, msaaDepthRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERROR: MSAA framebuffer incomplete\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// === Crear framebuffer para post-procesado (sharpen) ===
	glGenFramebuffers(1, &sharpenFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, sharpenFBO);

	// Textura color single-sample
	glGenTextures(1, &sharpenTexture);
	glBindTexture(GL_TEXTURE_2D, sharpenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sharpenTexture, 0);

	// Depth-stencil renderbuffer single-sample
	glGenRenderbuffers(1, &sharpenRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, sharpenRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, sharpenRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERROR: Sharpen framebuffer incomplete\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Application::InitCrosshair() {
	crosshairShader = new Shader("shaders/crosshair.vert", "shaders/crosshair.frag");
	float s = 5.0f;
	float hx = s / float(width) * 2.0f;
	float hy = s / float(height) * 2.0f;
	float verts[] = { -hx, 0, hx, 0, 0, -hy, 0, hy };
	glGenVertexArrays(1, &crossVAO);
	glGenBuffers(1, &crossVBO);
	glBindVertexArray(crossVAO);
	glBindBuffer(GL_ARRAY_BUFFER, crossVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindVertexArray(0);
}

void Application::InitPostProcessing() {
	postShader = new Shader("shaders/sharpen.vert", "shaders/sharpen.frag");
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	glGenVertexArrays(1, &postVAO);
	glGenBuffers(1, &postVBO);
	glBindVertexArray(postVAO);
	glBindBuffer(GL_ARRAY_BUFFER, postVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);
}

int Application::Run() {
	using clock = std::chrono::steady_clock;
	auto nextFrame = clock::now();

	while (!glfwWindowShouldClose(window)) {
		auto frameStart = clock::now();

		ProcessInput();
		Update();
		Render();

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (fpsCapOption != 4 && invFrameTime.count() > 0) {
			nextFrame += invFrameTime;
			auto almost = nextFrame - std::chrono::microseconds(1000);
			std::this_thread::sleep_until(almost);
			while (clock::now() < nextFrame) {}
		}
		else {
			nextFrame = clock::now();
		}

		frameCount++;
		double currentTime = glfwGetTime();
		if (currentTime - lastTime >= 1.0) {
			std::cout << "FPS: " << frameCount << std::endl;
			frameCount = 0;
			lastTime = currentTime;
		}
	}
	return 0;
}

void Application::ProcessInput() {
	// Control dinámico de nitidez
	bool increase = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
	bool decrease = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
	if (increase) sharpness = glm::min(sharpness + 0.1f, 1.0f);
	if (decrease) sharpness = glm::max(sharpness - 0.1f, 0.0f);
}

void Application::Update() {
	float delta = static_cast<float>(glfwGetTime());
	scene->Update(delta);
}

void Application::Render() {
	// 1) Render a MSAA framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, msaaFBO);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = camera->GetProjection();
	glm::mat4 view = camera->GetView();
	glDepthMask(GL_FALSE);
	sky->Draw(projection, view);
	glDepthMask(GL_TRUE);
	scene->Render(projection, view);

	// 2) Resolve MSAA to single-sample FBO
	glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, sharpenFBO);
	glBlitFramebuffer(0, 0, width, height,
		0, 0, width, height,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 3) Post-procesado: dibujar quad con textura resultante
	glClear(GL_COLOR_BUFFER_BIT);
	postShader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sharpenTexture);
	postShader->SetUniform("screenTexture", 0);
	postShader->SetUniform("sharpness", sharpness);
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(postVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);

	// 4) Crosshair
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