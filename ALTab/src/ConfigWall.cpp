// File: ConfigWall.cpp
#include "ConfigWall.hpp"
#include <glm/glm.hpp>

ConfigWall::ConfigWall()
	: configShader_("shaders/configwall.vert", "shaders/configwall.frag") {
	// Inicializar botones (VAOs, VBOs) y estados por defecto
	// lockButton_, modeButtons_, countPlusButton_, countMinusButton_, clearButton_
}

ConfigWall::~ConfigWall() {
	// Liberar VAOs/VBOs
}

void ConfigWall::render() {
	configShader_.use();
	// Render lockButton_ (usar config_.locked)
	// Render toggles shrink y microshot
	// Render modeButtons_ (destacar config_.mode)
	// Render count buttons (opacos si mode != Multishot)
	// Render clearButton_
}

void ConfigWall::updateInput() {
	if (config_.locked) return;
	// Raycast desde cursor, detectar clicks en botones:
	// if (hit lockButton_) toggleLock();
	// if (hit shrink) toggleShrink();
	// if (hit micro) toggleMicroshot();
	// if (hit modeButtons_[i]) selectMode(...);
	// if (hit plus/minus) incrementCount()/decrementCount();
	// if (hit clear) clearTargetsTrigger();
}

const AimWallConfig& ConfigWall::getConfig() const {
	return config_;
}

void ConfigWall::toggleLock() {
	config_.locked = !config_.locked;
}

void ConfigWall::toggleShrink() {
	config_.sizeShrink = !config_.sizeShrink;
}

void ConfigWall::toggleMicroshot() {
	config_.microshot = !config_.microshot;
}

void ConfigWall::selectMode(AimWallConfig::Mode mode) {
	config_.mode = mode;
}

void ConfigWall::incrementCount() {
	if (config_.count < 10) ++config_.count;
}

void ConfigWall::decrementCount() {
	if (config_.count > 1) --config_.count;
}

void ConfigWall::clearTargetsTrigger() {
	// Llamar a AimWallManager::clearTargets si mantienes referencia
}