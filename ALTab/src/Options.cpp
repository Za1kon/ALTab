#include "Options.hpp"

Options::Options() : wall(nullptr) {}

Options::~Options() {
	delete wall;
}

void Options::Init() {
	wall = new Wall(
		glm::vec3(0.0f, 0.0f, 5.0f),      // detrás
		glm::vec2(6.0f, 4.0f),
		"shaders/basic.vert",
		"shaders/basic.frag"
	);
}

void Options::Update(float) {}

void Options::Render(const glm::mat4& projection, const glm::mat4& view) {
	wall->Draw(projection, view);
}