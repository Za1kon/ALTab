// File: AimWallManager.cpp
#include "AimWallManager.hpp"
#include "ConfigWall.hpp" // Solo si necesita referencias
#include "Target.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>  // para glm::linearRand

AimWallManager::AimWallManager()
	: targetShader_("shaders/aimtarget.vert", "shaders/aimtarget.frag") {
	// Pre-reservar pool de 10 blancos
	for (int i = 0; i < 10; ++i) {
		auto t = new Target();
		t->setActive(false);
		pool_.push_back(t);
	}
}

AimWallManager::~AimWallManager() {
	for (auto t : pool_) delete t;
}

glm::vec3 AimWallManager::randomPositionOnWall() {
	// Rango asumido según tamaño del AimWall: ancho 9, alto 6, z un poco delante (-4.5)
	float x = glm::linearRand(-4.5f, 4.5f);
	float y = glm::linearRand(-3.0f, 3.0f);
	float z = -4.5f;
	return glm::vec3(x, y, z);
}

void AimWallManager::spawnTargetsFromConfig(const AimWallConfig& cfg) {
	if (cfg.locked) return;
	if (cfg.mode != currentConfig_.mode || cfg.count != currentConfig_.count
		|| cfg.sizeShrink != currentConfig_.sizeShrink || cfg.microshot != currentConfig_.microshot) {
		clearTargets();
		switch (cfg.mode) {
		case AimWallConfig::Multishot:      spawnMultishot(cfg); break;
		case AimWallConfig::Classic3x3:     spawnClassic3x3(cfg); break;
		case AimWallConfig::HorizontalRow:  spawnHorizontalRow(cfg); break;
		case AimWallConfig::FlickPairs:     spawnFlickPairs(cfg); break;
		case AimWallConfig::LongTravel:     spawnLongTravel(cfg); break;
		}
		currentConfig_ = cfg;
	}
	// Renderizado de todos los blancos activos
	targetShader_.use();
	// Configurar uniforms comunes
	targetShader_.setBool("uMicroshot", cfg.microshot);
	targetShader_.setVec3("uBaseColor", glm::vec3(0.3f, 0.6f, 1.0f));
	for (auto t : pool_) {
		if (t->isActive()) t->draw(targetShader_);
	}
}

void AimWallManager::clearTargets() {
	for (auto t : pool_) t->setActive(false);
}

void AimWallManager::spawnMultishot(const AimWallConfig& cfg) {
	// Posicionar cfg.count blancos al azar
	int used = 0;
	for (auto t : pool_) {
		if (used >= cfg.count) break;
		t->setPosition(randomPositionOnWall());
		float scale = cfg.sizeShrink ? (1.0f - float(used) / cfg.count * 0.5f) : 1.0f;
		if (cfg.microshot) scale *= 0.2f;
		t->setScale(scale);
		t->setActive(true);
		++used;
	}
}

void AimWallManager::spawnClassic3x3(const AimWallConfig& cfg) {
	// Implementar grilla 3x3
}

void AimWallManager::spawnHorizontalRow(const AimWallConfig& cfg) {
	// Implementar fila horizontal
}

void AimWallManager::spawnFlickPairs(const AimWallConfig& cfg) {
	// Implementar flick pairs
}

void AimWallManager::spawnLongTravel(const AimWallConfig& cfg) {
	// Implementar long travel
}