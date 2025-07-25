// File: AimWallManager.hpp
#pragma once
#include "AimWallConfig.hpp"
#include "ShaderProgram.hpp"   // Asume utilidades de carga de shaders
#include <vector>

class Target;

class AimWallManager {
public:
	AimWallManager();
	~AimWallManager();

	// Genera y posiciona los blancos según la configuración
	void spawnTargetsFromConfig(const AimWallConfig& config);
	void clearTargets();

private:
	void spawnMultishot(const AimWallConfig& cfg);
	void spawnClassic3x3(const AimWallConfig& cfg);
	void spawnHorizontalRow(const AimWallConfig& cfg);
	void spawnFlickPairs(const AimWallConfig& cfg);
	void spawnLongTravel(const AimWallConfig& cfg);

	// Pool de blancos para reutilización
	std::vector<Target*> pool_;
	AimWallConfig currentConfig_;

	glm::vec3 randomPositionOnWall();

	// Shader para dibujar los blancos
	ShaderProgram targetShader_;
};