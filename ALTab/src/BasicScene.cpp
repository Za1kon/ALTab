// BasicScene.cpp
#include "BasicScene.hpp"
#include <glm/gtc/matrix_transform.hpp>

BasicScene::BasicScene(float size, int resolution)
	: terrain(nullptr), shader(nullptr), size(size), resolution(resolution) {
}

BasicScene::~BasicScene() {
	delete terrain;
	delete shader;
}

void BasicScene::Init() {
	terrain = new Terrain(size, resolution);
	shader = new Shader("shaders/basic.vert", "shaders/basic.frag");
}

void BasicScene::Update(float) {
	// no-op for static terrain
}

void BasicScene::Render(const glm::mat4& viewProj) {
	shader->Use();
	glm::mat4 model = glm::mat4(1.0f);  // terrain at origin
	glm::mat4 mvp = viewProj * model;
	shader->SetUniform("uMVP", mvp);
	terrain->Draw();
}
