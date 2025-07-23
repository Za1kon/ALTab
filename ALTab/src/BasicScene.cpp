// ===== BasicScene.cpp =====
#include "BasicScene.hpp"

BasicScene::BasicScene(float size, int resolution)
	: platform(nullptr), shader(nullptr),
	aimWall(nullptr), optionsWall(nullptr),
	size(size), resolution(resolution) {
}

BasicScene::~BasicScene() {
	delete platform;
	delete shader;
	delete aimWall;
	delete optionsWall;
}

void BasicScene::Init() {
	platform = new Platform(size, resolution);
	shader = new Shader("shaders/platform.vert", "shaders/platform.frag");

	aimWall = new Wall({ 0.0f, 1.5f, -5.0f }, { 4.0f, 3.0f }, "shaders/basic.vert", "shaders/basic.frag");
	optionsWall = new Wall({ 0.0f, 1.5f, 5.0f }, { 4.0f, 3.0f }, "shaders/basic.vert", "shaders/basic.frag");
}

void BasicScene::Update(float) {}

void BasicScene::Render(const glm::mat4& projection, const glm::mat4& view) {
	glm::mat4 mvp = projection * view * glm::mat4(1.0f);

	// platform
	shader->Use();
	shader->SetUniform("uMVP", mvp);
	platform->Draw();

	// Walls
	aimWall->Draw(projection, view);
	optionsWall->Draw(projection, view);
}