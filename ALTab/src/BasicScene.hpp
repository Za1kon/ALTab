// ===== BasicScene.hpp =====
#pragma once
#include "Scene.hpp"
#include "Platform.hpp"
#include "Shader.hpp"
#include "Wall.hpp"
#include <glm/glm.hpp>

class BasicScene : public Scene {
public:
	BasicScene(float size = 10.0f, int resolution = 100);
	~BasicScene();

	void Init() override;
	void Update(float deltaTime) override;
	void Render(const glm::mat4& projection, const glm::mat4& view) override;

private:
	Platform* platform;
	Shader* shader;
	Wall* aimWall;
	Wall* optionsWall;
	float    size;
	int      resolution;
};