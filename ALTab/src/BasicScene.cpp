// ===== BasicScene.cpp =====
#include "BasicScene.hpp"

BasicScene::BasicScene(float size, int resolution)
	: terrain(nullptr), shader(nullptr), sky(nullptr), size(size), resolution(resolution) {
}

BasicScene::~BasicScene() {
	delete terrain;
	delete shader;
	delete sky;
}

void BasicScene::Init() {
	terrain = new Terrain(size, resolution);
	shader = new Shader("shaders/basic.vert", "shaders/basic.frag");
	sky = new SkyDome("assets/skydome.hdr", size);
}

void BasicScene::Update(float /* deltaTime */) {
	// no dynamic content for now
}

void BasicScene::Render(const glm::mat4& projection, const glm::mat4& view) {
	// draw skydome
	glDepthMask(GL_FALSE); // no escribir en el depth buffer
	sky->Draw(projection, view);
	glDepthMask(GL_TRUE);  // reactivar escritura después

	// draw terrain
	shader->Use();
	glm::mat4 mvp = projection * view * glm::mat4(1.0f);
	shader->SetUniform("uMVP", mvp);
	terrain->Draw();
}