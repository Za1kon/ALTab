// ===== BasicScene.hpp =====
#pragma once
#include "Scene.hpp"
#include "Shader.hpp"
#include "Platform.hpp"
#include "Wall.hpp"
#include <glm/glm.hpp>

class BasicScene : public Scene {
public:
	BasicScene(float terrainSize, int terrainRes);
	~BasicScene();

	void Init() override;
	void Update(float delta) override;
	void Render(const glm::mat4& projection, const glm::mat4& view) override;

private:
	// Parámetros de terreno (usados para la plataforma)
	float terrainSize;
	int terrainRes;

	// Plataforma estilizada
	Shader* platformShader;
	Platform* platform;

	// Paredes frontales y traseras
	Wall* aimWall;
	Wall* optionsWall;

	BasicScene(const BasicScene&) = delete;
	BasicScene& operator=(const BasicScene&) = delete;
};