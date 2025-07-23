#include "AimTrainer.hpp"

AimTrainer::AimTrainer() : wall(nullptr) {}

AimTrainer::~AimTrainer() {
	delete wall;
}

void AimTrainer::Init() {
	wall = new Wall(
		glm::vec3(0.0f, 0.0f, -5.0f),     // en frente
		glm::vec2(6.0f, 4.0f),
		"shaders/basic.vert",
		"shaders/basic.frag"
	);
}

void AimTrainer::Update(float) {}

void AimTrainer::Render(const glm::mat4& projection, const glm::mat4& view) {
	wall->Draw(projection, view);
}