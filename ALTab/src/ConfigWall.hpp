// File: ConfigWall.hpp
#pragma once
#include "AimWallConfig.hpp"
#include "ShaderProgram.hpp"
#include <vector>

class ConfigWall {
public:
	ConfigWall();
	~ConfigWall();

	// Dibuja la pared de configuración y procesa entradas
	void render();
	void updateInput();

	const AimWallConfig& getConfig() const;

private:
	AimWallConfig config_;

	// Geometría y shaders
	ShaderProgram configShader_;
	struct Button { unsigned int vao, vbo; AimWallConfig::Mode mode; bool active; };
	Button lockButton_;
	std::vector<Button> modeButtons_;
	unsigned int countPlusButton_, countMinusButton_, clearButton_;

	// Métodos internos para cada control
	void toggleLock();
	void toggleShrink();
	void toggleMicroshot();
	void selectMode(AimWallConfig::Mode mode);
	void incrementCount();
	void decrementCount();
	void clearTargetsTrigger();
};