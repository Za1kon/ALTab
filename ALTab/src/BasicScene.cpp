// ===== BasicScene.cpp =====
#include "BasicScene.hpp"

BasicScene::BasicScene(float size, int resolution)
	: platform(nullptr),
	platformShader(nullptr),
	aimWall(nullptr),
	optionsWall(nullptr),
	terrainSize(size),
	terrainRes(resolution) {
}

BasicScene::~BasicScene() {
	delete platform;
	delete platformShader;
	delete aimWall;
	delete optionsWall;
}

void BasicScene::Init() {
	platform = new Platform(terrainSize, terrainRes);
	platformShader = new Shader("shaders/platform.vert", "shaders/platform.frag");

	aimWall = new Wall({ 0.0f, 1.5f, -5.0f }, { 9.0f, 6.0f }, "shaders/wall.vert", "shaders/wall.frag");
	optionsWall = new Wall({ 0.0f, 1.5f,  5.0f }, { 4.0f, 3.0f }, "shaders/wall.vert", "shaders/wall.frag");
}

void BasicScene::Update(float) {
	// (sin animaciones por ahora)
}

void BasicScene::Render(const glm::mat4& projection, const glm::mat4& view) {
	// 1) Dibujar plataforma estilizada
	platformShader->Use();
	glm::mat4 model = glm::mat4(1.0f);

	platformShader->SetUniform("uModel", model);
	platformShader->SetUniform("uView", view);
	platformShader->SetUniform("uProjection", projection);

	// Valores de estilo (opcionalmente ajustables)
	platformShader->Use();
	platformShader->SetUniform("platformTex", 0);

	platform->Draw();

	// Estilo AimTrainer
	WallStyle aimStyle = {
		glm::vec3(0.005f,0.005f,0.03f),
		glm::vec3(0.1f,0.1f,0.2f),      // líneas suaves grises
		glm::vec3(0.02f,0.02f,0.05f),   // acento muy sutil
		0.02f,                           // borderSize
		glm::vec2(18.0f,12.0f),          // gridCount X,Y
		0.002f,                          // lineWidth
		0.03f                           // glowWidth
	};
	aimWall->SetStyle(aimStyle);
	aimWall->Draw(projection, view);

	// Estilo Options
	WallStyle optStyle = {
		glm::vec3(0.005f,0.005f,0.03f),  // bg
		glm::vec3(0.0f,0.5f,1.0f),       // lineColor azul vivo
		glm::vec3(0.02f,0.04f,0.1f),     // accent
		0.03f,
		glm::vec2(8.0f,6.0f),           // menos paneles
		0.005f,
		0.01f
	};
	optionsWall->SetStyle(optStyle);
	optionsWall->Draw(projection, view);
}