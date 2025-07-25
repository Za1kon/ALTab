// ==== Target.cpp ====
#include "Target.hpp"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <vector>

Target::Target() : position_(0.0f), scale_(1.0f), active_(false) {
	initMesh();
}

Target::~Target() {
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &vbo_);
}

void Target::setPosition(const glm::vec3& pos) { position_ = pos; }
void Target::setScale(float s) { scale_ = s; }
void Target::setActive(bool a) { active_ = a; }
bool Target::isActive() const { return active_; }

void Target::draw(const ShaderProgram& shader) const {
	if (!active_) return;

	glm::mat4 model = glm::translate(glm::mat4(1.0f), position_);
	model = glm::scale(model, glm::vec3(scale_));
	shader.setMat4("uModel", model);

	glBindVertexArray(vao_);
	glDrawArrays(GL_TRIANGLES, 0, sphereVertexCount);
	glBindVertexArray(0);
}

void Target::initMesh() {
	std::vector<float> vertices;
	const int latDiv = 10;
	const int lonDiv = 10;

	for (int i = 0; i <= latDiv; ++i) {
		float theta1 = float(i) / latDiv * glm::pi<float>();
		float theta2 = float(i + 1) / latDiv * glm::pi<float>();

		for (int j = 0; j <= lonDiv; ++j) {
			float phi1 = float(j) / lonDiv * glm::two_pi<float>();
			float phi2 = float(j + 1) / lonDiv * glm::two_pi<float>();

			glm::vec3 p1 = {
				sin(theta1) * cos(phi1),
				cos(theta1),
				sin(theta1) * sin(phi1)
			};
			glm::vec3 p2 = {
				sin(theta2) * cos(phi1),
				cos(theta2),
				sin(theta2) * sin(phi1)
			};
			glm::vec3 p3 = {
				sin(theta2) * cos(phi2),
				cos(theta2),
				sin(theta2) * sin(phi2)
			};
			glm::vec3 p4 = {
				sin(theta1) * cos(phi2),
				cos(theta1),
				sin(theta1) * sin(phi2)
			};

			auto push = [&](glm::vec3 v) {
				vertices.push_back(v.x);
				vertices.push_back(v.y);
				vertices.push_back(v.z);
				};

			push(p1); push(p2); push(p3);
			push(p1); push(p3); push(p4);
		}
	}

	sphereVertexCount = static_cast<int>(vertices.size() / 3);

	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);

	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);
}