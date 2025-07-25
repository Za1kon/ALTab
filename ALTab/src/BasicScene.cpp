// File: BasicScene.cpp
#include "BasicScene.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

BasicScene::BasicScene(float size, int resolution)
	: platform(nullptr),
	platformShader(nullptr),
	terrainSize(size),
	terrainRes(resolution),
	showAimWalls_(false) {
}

BasicScene::~BasicScene() {
	delete platform;
	delete platformShader;
}

void BasicScene::Init() {
	platform = new Platform(terrainSize, terrainRes);
	platformShader = new Shader("shaders/platform.vert", "shaders/platform.frag");

	aimWallManager_ = std::make_unique<AimWallManager>();
	configWall_ = std::make_unique<ConfigWall>();
}

void BasicScene::Update(float) {
	// sin animaciones aún
}

void BasicScene::HandleInput() {
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS &&
		(glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)) {
		showAimWalls_ = !showAimWalls_;
		glfwSetInputMode(window, GLFW_CURSOR, showAimWalls_ ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}
	if (showAimWalls_) {
		configWall_->updateInput();
	}
}

void BasicScene::Render(const glm::mat4& projection, const glm::mat4& view) {
	platformShader->Use();
	glm::mat4 model = glm::mat4(1.0f);
	platformShader->SetUniform("uModel", model);
	platformShader->SetUniform("uView", view);
	platformShader->SetUniform("uProjection", projection);
	platformShader->SetUniform("platformTex", 0);
	platform->Draw();

	if (showAimWalls_) {
		configWall_->render();
		aimWallManager_->spawnTargetsFromConfig(configWall_->getConfig());
	}
}